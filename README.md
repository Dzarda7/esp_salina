# esp_salina

A public transport departure board on a
[LaskaKit ESPink-Shelf-2.9](https://www.laskakit.cz/laskakit-espink-shelf-2-9-esp32-e-paper/)
(ESP32 + a 2.9" GDEY029T94 e-paper panel).

It wakes on a timer, joins Wi-Fi, fetches the next departures, draws two
platforms side by side, refreshes the panel and goes back to deep sleep.

Two networks are built in and picked during setup: **IDS JMK**, covering Brno
and the rest of South Moravia, and **PID** for Prague, through the Golemio API.

![Departures on the panel](docs/departures.jpeg)

The panel is driven by [`dzarda7/esp_epaper`](https://components.espressif.com/components/dzarda7/esp_epaper),
a separate component that is not tied to this display. Moving to another
e-paper panel is mostly a matter of pointing `BOARD_EPD_PANEL` at a different
descriptor - see [Porting](#porting).

## Building

Needs ESP-IDF v6.0 or newer; developed against v6.2.

```sh
idf.py set-target esp32
idf.py menuconfig      # intervals, battery, optional config pin
idf.py flash monitor
```

## Setup

Wi-Fi and the stop are not compiled in. A board with nothing stored raises its
own access point and shows how to reach it, so the binary carries no
credentials and moving to another network needs no rebuild.

1. Flash and power up. The screen shows a QR code and the network name.

   ![The setup screen](docs/setup.jpeg)

2. Scan the QR, or join the open `esp_salina` network by hand. The setup page
   should open by itself; if it does not, go to `http://192.168.4.1`.
3. Pick your Wi-Fi and enter its password. The board joins while keeping the
   setup network alive, so the page stays reachable - it may blink out for a
   second as the radio changes channel.
4. Choose the transport network. Prague also asks for an API key here; the
   form says where to get one.
5. Type the stop name, then pick a platform for each column from the ones it
   found and save. The board restarts and starts showing departures.

How forgiving the stop name is depends on which one you picked:

| | Matching | Platforms | Key |
|---|---|---|---|
| IDS JMK | loose - `kartouzska` finds `Kartouzská`, partial names work | numbered, with a direction | none |
| PID | **exact**, including capitals and accents | lettered (`A`, `J`) or numbered, no direction text | yours, free |

For IDS JMK any stop in the area resolves, not just Brno ones - `znojmo`,
`blansko` and `kurim` all work - and the result names the town, so check it
before saving. PID has no fuzzy matching of any kind, so `malostranska` finds
nothing and `Malostranská` is required.

The portal stays up until you finish it, so an unprovisioned board keeps its
radio on. Power it from USB while setting it up rather than leaving it on a
battery.

The settings live in NVS. To change them, either erase NVS over the cable or
set `SALINA_CONFIG_PIN` to a GPIO and hold a button on it at boot. Not GPIO0 -
low at reset puts the ESP32 into ROM download mode.

On IDS JMK a platform only exists in the API while it has upcoming departures,
so setting up in the middle of the night can show a stop with no platforms to
choose. The same answer turns up in short bursts at any hour; searching again
clears it.

Prague keys are per person - the Golemio terms forbid sharing one across
devices - which is why the key is asked for during setup rather than being
built in.

## Configuration

The rest is in `idf.py menuconfig` under *Salina departure display*: the update
interval, retry attempts, the night window and the battery divider.

## Design notes

**Departure times are absolute, not countdowns.** The API answers either
`16:03` or `7min`; the countdown is only true at the instant of the request and
is stale by the time the panel next refreshes, so it is converted to the wall
clock time of the departure. Without a synced clock it falls back to the
countdown rather than printing a time it cannot justify.

**A failed update leaves the screen alone.** E-paper holds its image without
power, so a Wi-Fi or API failure keeps the last good departures on the glass
rather than painting an error page. The API sometimes answers with the stop but
no platforms, so the fetch retries a few times while Wi-Fi is still up; only
after those does the wake give up and wait for the next one. The `upd.` stamp
therefore always tells you when the data on screen was actually fetched.

## Layout

The layout is derived from whatever size the driver reports: column width from
the panel width, row count from `(height - header) / row height`. A taller panel
simply shows more departures. Fonts are fixed sizes, generated as 1bpp subsets.

Within a column: line number, a gutter, the destination (ellipsised when it does
not fit), and the departure time. The time and line number are never shortened -
a time cut to `16:0…` is worse than no time at all - so they get columns wide
enough for their longest value, measured from the font.

## Porting

| To change | Edit |
|---|---|
| Board pins, panel model, orientation | `main/board.h` |
| Add a city | a new file next to `main/pid.c` |
| Screen layout | `main/ui.c` |
| Sleep and retry policy | `main/main.c` |

A city is one file exporting a `salina_source_t` (`main/salina.h`): a `lookup()`
that turns a typed name into platforms for the setup form, and a `fetch()` that
fills `salina_data_t`. Register it in `main/source.c` and it appears in the
setup dropdown; nothing else knows which network is in use. `main/http.c` has
the HTTPS GET, with an optional `X-Access-Token` for APIs that want a key.

The two shipped sources show the range: IDS JMK numbers its platforms and
matches names loosely, PID labels them with letters and demands exact names, so
the column selector is stored as text - `"1"` in one case, a GTFS stop id like
`"U360Z1P"` in the other.

The display driver is a separate component,
[`dzarda7/esp_epaper`](https://components.espressif.com/components/dzarda7/esp_epaper),
pulled from the registry by the component manager. It is not specific to this
panel.

Swapping in a panel the component does not ship does not need a fork either -
`epd_panel_desc_t` is public, so a descriptor can live in `main/` beside
`board.h` and `BOARD_EPD_PANEL` can point at it.

## Fonts

`main/fonts/` holds 1bpp Montserrat subsets (ASCII, Czech diacritics, and a few
FontAwesome glyphs), generated with `lv_font_conv`:

```sh
npx lv_font_conv --bpp 1 --size 14 \
  --font Montserrat-Medium.ttf -r '0x20-0x7F' \
  --symbols "ÁáČčĎďÉéĚěÍíŇňÓóŘřŠšŤťÚúŮůÝýŽž" \
  --font FontAwesome5-Solid+Brands+Regular.woff -r "61931,62016,62017,62018,62019,62020" \
  --format lvgl -o main/fonts/font_salina_14.c --force-fast-kern-format --lv-include lvgl.h
```

1bpp rather than anti-aliased: on a monochrome panel the grey levels would only
be dithered away.
