<!-- markdownlint-disable MD033 -->
<!-- markdownlint-disable MD041 -->

<div align="center">
  <h2 align="center">💧 Arduino library for Bosch Sensortec BME280</h2>
  <p align="center">
    The library allows using the sensor via
    <a href="https://www.i2c-bus.org" aria-label="I2C Bus">I2C</a>
    or
    <a href="https://en.wikipedia.org/wiki/Serial_Peripheral_Interface"
      aria-label="SPI">SPI</a>
    on any board that supports
    <a href="https://platformio.org/frameworks/arduino"
      aria-label="Arduino Framework">Arduino</a>.
  </p>

  <p id="shields" align="center" markdown="1">

[![License](https://img.shields.io/badge/license-MIT-3178C6?style=flat)](LICENSE)
[![Style Guide](https://img.shields.io/badge/code%20style-~google-4285F4?style=flat)][google-cppguide]
![clang-format](https://img.shields.io/badge/formatter-clang--format-262D3A?style=flat)
[![markdownlint](https://img.shields.io/badge/linter-markdownlint-000?style=flat)][github-markdownlint]
[![Tested on Arduino Uno](https://img.shields.io/badge/tested%20on-arduino%20uno-00979D)][arduino-uno]
[![Tested on Arduino Nano](https://img.shields.io/badge/tested%20on-arduino%20nano-00979D)][arduino-nano]
[![Tested on NodeMCU](https://img.shields.io/badge/tested%20on-nodemcu-E7352C)][nodemcu]
[![Tested on Wemos D1](https://img.shields.io/badge/tested%20on-wemos%20d1-E7352C)][wemos-d1]
![Code Coverage](https://img.shields.io/badge/coverage-99.5%25-brightgreen)
![CI Workflow](https://github.com/malokhvii-eduard/arduino-bme280/actions/workflows/ci.yml/badge.svg)

  </p>
</div>

---

## :tada: Features

- Easy to use, Zero Dependencies, Ultra Light-Weight
- Implements communication via [I2C][i2c] or [SPI][spi]
- Tested on: [Arduino Uno][arduino-uno], [Arduino Nano][arduino-nano],
[NodeMCU][nodemcu], [Wemos D1][wemos-d1]

## :sparkles: Getting Started

### :package: Installation

<details>
  <summary>
    👉 via PlatformIO
  </summary>
  <br>

Add `https://github.com/malokhvii-eduard/arduino-bme280.git` to
[`lib_deps`][platformio-lib-deps] in `platformio.ini`. Then run in your project
root [`pio lib install`][platformio-lib-install]. Enjoy! :tada::+1:

</details>

<details>
  <summary>
    👉 via Arduino IDE
  </summary>
  <br>

  Comming soon... :construction:
</details>

<details>
  <summary>
    👉 as
    <a href="https://git-scm.com/book/en/v2/Git-Tools-Submodules"
      aria-label="Git Submodule">Submodule</a>
  </summary>
  <br>

Add [*Submodule*][git-scm-submodules] to your project (`git submodule add
https://github.com/malokhvii-eduard/arduino-bme280.git arduino-bme280`).
Enjoy! :tada::+1:

</details>

### :eyes: Usage

There are various [examples](https://github.com/malokhvii-eduard/arduino-bme280/tree/master/examples)
to demonstrate usage of the library. The examples are listed below:

- [two-wire.ino](examples/two-wire/two-wire.ino) &ndash; read temperature,
pressure, humidity via [I2C][i2c]
- [settings.ino](examples/settings/settings.ino) &ndash; set different settings

## :question: FAQs

<!-- FAQ 1 -->
<!-- markdownlint-disable MD013 -->
### :raising_hand_man: In which units temperature, pressure, humidity are measured?
<!-- markdownlint-enable MD013 -->

:point_right: The temperature is measured in degrees Celsius (*symbol:* `°C`).
The pressure is measured in pascals (*symbol:* `Pa`). The humidity is measured
in percentages (*symbol:* `%`).

<!-- FAQ 2 -->
### :raising_hand_man: What settings are used by default?

:point_right: The default settings are:

- mode &ndash; `Bme280Mode::Normal`
- temperature oversampling &ndash; `Bme280Oversampling::X16`
- pressure oversampling &ndash; `Bme280Oversampling::X16`
- humidity oversampling &ndash; `Bme280Oversampling::X16`
- filter &ndash; `Bme280Filter::Off`
- standby time &ndash; `Bme280StandbyTime::Ms0_5`

<!-- FAQ 3 -->
### :raising_hand_man: Are there any settings presets out-of-the-box?

:point_right: Yes. There are some settings presets out-of-the-box according to
the recomendations in the [datasheet][bosch-sensortec-bme280]. You can create
settings preset by call one of factory methods in `Bme280Settings`. The
factory methods are listed below:

- `Bme280Settings::indoor()`
- `Bme280Settings::weatherMonitoring()`
- `Bme280Settings::gaming()`
- `Bme280Settings::humiditySensing()`

<!-- FAQ 4 -->
### :raising_hand_man: How to off one of any measurements?

:point_right: Assign `Bme280Oversampling::Off` to an oversampling in settings.

<!-- FAQ 5 -->
### :raising_hand_man: How does it work?

:point_right: You can explore [unit tests](https://github.com/malokhvii-eduard/arduino-bme280/tree/master/test/native)
to see the behavior of all library members.

## :pushpin: Roadmap

- [ ] Add implementation for [SPI][spi] (`Bme280FourWire`)
- [ ] Add archive with library source for [Arduino IDE][github-arduino-ide]
- [ ] Upload to [PlatformIO Registry][platformio-registry]

## :hammer_and_wrench: Tech Stack

<!-- markdownlint-disable MD013 -->
[![EditorConfig](https://img.shields.io/badge/EditorConfig-FEFEFE?logo=editorconfig&logoColor=000&style=flat)][editorconfig]
![Markdown](https://img.shields.io/badge/Markdown-000?logo=markdown&logoColor=fff&style=flat)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=flat)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?logo=data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjUwMCIgaGVpZ2h0PSIyNTAwIiB2aWV3Qm94PSIwIDAgMjU2IDI1NiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCI+PHBhdGggZD0iTTEyOCAwQzkzLjgxIDAgNjEuNjY2IDEzLjMxNCAzNy40OSAzNy40OSAxMy4zMTQgNjEuNjY2IDAgOTMuODEgMCAxMjhjMCAzNC4xOSAxMy4zMTQgNjYuMzM0IDM3LjQ5IDkwLjUxQzYxLjY2NiAyNDIuNjg2IDkzLjgxIDI1NiAxMjggMjU2YzM0LjE5IDAgNjYuMzM0LTEzLjMxNCA5MC41MS0zNy40OUMyNDIuNjg2IDE5NC4zMzQgMjU2IDE2Mi4xOSAyNTYgMTI4YzAtMzQuMTktMTMuMzE0LTY2LjMzNC0zNy40OS05MC41MUMxOTQuMzM0IDEzLjMxNCAxNjIuMTkgMCAxMjggMCIgZmlsbD0iI0ZGN0YwMCIvPjxwYXRoIGQ9Ik0yNDkuMzg2IDEyOGMwIDY3LjA0LTU0LjM0NyAxMjEuMzg2LTEyMS4zODYgMTIxLjM4NkM2MC45NiAyNDkuMzg2IDYuNjEzIDE5NS4wNCA2LjYxMyAxMjggNi42MTMgNjAuOTYgNjAuOTYgNi42MTQgMTI4IDYuNjE0YzY3LjA0IDAgMTIxLjM4NiA1NC4zNDYgMTIxLjM4NiAxMjEuMzg2IiBmaWxsPSIjRkZGIi8+PHBhdGggZD0iTTE2MC44NjkgNzQuMDYybDUuMTQ1LTE4LjUzN2M1LjI2NC0uNDcgOS4zOTItNC44ODYgOS4zOTItMTAuMjczIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzJzLTEwLjMyIDQuNjItMTAuMzIgMTAuMzJjMCAzLjc1NSAyLjAxMyA3LjAzIDUuMDEgOC44MzdsLTUuMDUgMTguMTk1Yy0xNC40MzctMy42Ny0yNi42MjUtMy4zOS0yNi42MjUtMy4zOWwtMi4yNTggMS4wMXYxNDAuODcybDIuMjU4Ljc1M2MxMy42MTQgMCA3My4xNzctNDEuMTMzIDczLjMyMy04NS4yNyAwLTMxLjYyNC0yMS4wMjMtNDUuODI1LTQwLjU1NS01Mi4xOTd6TTE0Ni41MyAxNjQuOGMtMTEuNjE3LTE4LjU1Ny02LjcwNi02MS43NTEgMjMuNjQzLTY3LjkyNSA4LjMyLTEuMzMzIDE4LjUwOSA0LjEzNCAyMS41MSAxNi4yNzkgNy41ODIgMjUuNzY2LTM3LjAxNSA2MS44NDUtNDUuMTUzIDUxLjY0NnptMTguMjE2LTM5Ljc1MmE5LjM5OSA5LjM5OSAwIDAgMC05LjM5OSA5LjM5OSA5LjM5OSA5LjM5OSAwIDAgMCA5LjQgOS4zOTkgOS4zOTkgOS4zOTkgMCAwIDAgOS4zOTgtOS40IDkuMzk5IDkuMzk5IDAgMCAwLTkuMzk5LTkuMzk4em0yLjgxIDguNjcyYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDkgMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OXoiIGZpbGw9IiNFNTcyMDAiLz48cGF0aCBkPSJNMTAxLjM3MSA3Mi43MDlsLTUuMDIzLTE4LjkwMWMyLjg3NC0xLjgzMiA0Ljc4Ni01LjA0IDQuNzg2LTguNzAxIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzItNS42OTkgMC0xMC4zMTkgNC42Mi0xMC4zMTkgMTAuMzIgMCA1LjY4MiA0LjU5MiAxMC4yODkgMTAuMjY3IDEwLjMxN0w5NS44IDc0LjM3OGMtMTkuNjA5IDYuNTEtNDAuODg1IDIwLjc0Mi00MC44ODUgNTEuODguNDM2IDQ1LjAxIDU5LjU3MiA4NS4yNjcgNzMuMTg2IDg1LjI2N1Y2OC44OTJzLTEyLjI1Mi0uMDYyLTI2LjcyOSAzLjgxN3ptMTAuMzk1IDkyLjA5Yy04LjEzOCAxMC4yLTUyLjczNS0yNS44OC00NS4xNTQtNTEuNjQ1IDMuMDAyLTEyLjE0NSAxMy4xOS0xNy42MTIgMjEuNTExLTE2LjI4IDMwLjM1IDYuMTc1IDM1LjI2IDQ5LjM2OSAyMy42NDMgNjcuOTI2em0tMTguODItMzkuNDZhOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTkgOS4zOTggOS4zOTkgOS4zOTkgMCAwIDAgOS40IDkuNCA5LjM5OSA5LjM5OSAwIDAgMCA5LjM5OC05LjQgOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTktOS4zOTl6bS0yLjgxIDguNjcxYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDggMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OHoiIGZpbGw9IiNGRjdGMDAiLz48L3N2Zz4=&logoColor=fff&style=flat)][github-platformio]
[![Arduino](https://img.shields.io/badge/Arduino-00979D?logo=arduino&logoColor=fff&style=flat)][github-arduino]
[![Google Test](https://img.shields.io/badge/Google%20Test-4285F4?logo=google&logoColor=fff&style=flat)][github-googletest]
[![gcovr](https://img.shields.io/badge/gcovr-A42E2B?logo=gnu&logoColor=fff&style=flat)][github-gcovr]
![clang-format](https://img.shields.io/badge/clang--format-262D3A?logo=llvm&logoColor=fff&style=flat)
[![markdownlint](https://img.shields.io/badge/markdownlint-000?logo=markdown&logoColor=fff&style=flat)][github-markdownlint]
[![BME280](https://img.shields.io/badge/BME280-EA0016?logo=bosch&logoColor=fff&style=flat)][bosch-sensortec-bme280]
[![Shields.io](https://img.shields.io/badge/Shields.io-000?logo=shieldsdotio&logoColor=fff&style=flat)][shields]
[![Git](https://img.shields.io/badge/Git-F05032?logo=git&logoColor=fff&style=flat)][git-scm]
[![GitHub](https://img.shields.io/badge/GitHub-181717?logo=github&logoColor=fff&style=flat)][github]
[![GitHub Actions](https://img.shields.io/badge/GitHub%20Actions-2088FF?logo=githubactions&logoColor=fff&style=flat)][github-actions]
<!-- markdownlint-enable MD013 -->

## :writing_hand: Contributing

:+1::tada: *First off, thanks for taking the time to contribute!* :tada::+1:

Contributions are what make the open source community such an amazing place to
be learn, inspire, and create. Any contributions you make are **greatly
appreciated**.

1. Fork the *Project*
2. Create your *Feature Branch* (`git checkout -b feature/awesome-feature`)
3. Commit your *Changes* (`git commit -m 'Add awesome feature'`)
4. Push to the *Branch* (`git push origin feature/awesome-feature`)
5. Open a *Pull Request*

## :sparkling_heart: Like this project?

Leave a :star: if you think this project is cool or useful for you.

## :warning: License

`arduino-bme280` is licenced under the MIT License. See the [LICENSE](LICENSE)
for more information.

<!-- markdownlint-disable MD013 -->
<!-- Github links -->
[github-actions]: https://docs.github.com/en/actions
[github-arduino-ide]:  https://github.com/arduino/Arduino
[github-arduino]: https://github.com/arduino
[github-gcovr]: https://github.com/gcovr/gcovr
[github-googletest]: https://github.com/google/googletest
[github-linux]: https://github.com/torvalds/linux
[github-markdownlint]: https://github.com/DavidAnson/markdownlint
[github-platformio]: https://github.com/platformio
[github-simple-icons]: https://github.com/simple-icons/simple-icons
[github]: https://github.com

<!-- Other links -->
[arduino-nano]: https://store.arduino.cc/collections/boards/products/arduino-nano
[arduino-uno]: https://store.arduino.cc/products/arduino-uno-rev3
[bosch-sensortec-bme280-datasheet]: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
[bosch-sensortec-bme280]: https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280
[editorconfig]: https://editorconfig.org
[git-scm-submodules]: https://git-scm.com/book/en/v2/Git-Tools-Submodules
[git-scm]: https://git-scm.com
[google-cppguide]: https://google.github.io/styleguide/cppguide.html
[i2c]: https://www.i2c-bus.org
[nodemcu]: https://en.wikipedia.org/wiki/NodeMCU
[platformio-lib-deps]: https://docs.platformio.org/en/latest/projectconf/section_env_library.html#lib-deps
[platformio-lib-install]: https://docs.platformio.org/en/latest/core/userguide/lib/cmd_install.html
[platformio-registry]: https://platformio.org/lib
[shields]: https://shields.io
[spi]: https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
[wemos-d1]: https://www.wemos.cc/en/latest/d1/d1_mini.html