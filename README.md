# ATmega Gokan XIAO — Arduinoボードパッケージ

ATmega328PB / 外部16 MHzの互換基板用。D0～D10をXIAOの端子位置に合わせ、`PIN_WIRE0_SDA`などの機能別マクロを提供します。
対象回路は `atmega328pb-bmc-qfn`。別リビジョンはピン配線を確認してください。

## Arduino IDEへのインストール

Arduino IDE 2.x（内蔵Arduino CLI 1.0.4以降）を使用します。

1. **設定 → 追加のボードマネージャのURL** に次を追加します。

   ```text
   https://raw.githubusercontent.com/PenguinEino/arduino-xiao-atmega328pb/main/package_atmegagokan_index.json
   ```

2. ボードマネージャで **MiniCore 3.1.3** をインストールします。
3. **ATmega Gokan XIAO (install MiniCore first)** をインストールします。
4. ボードに **ATmega Gokan XIAO (ATmega328PB, 16 MHz)**、ポートに基板のUSBシリアルを選びます。
5. **ファイル → スケッチ例 → ATmegaGokan → Blink** でコンパイル・書き込みします。

登録URLは1つ、インストールするパッケージは2つです。MiniCore本体をこのパッケージに同梱していません。
同じURLにMiniCoreの公式配布先を参照するメタデータを収録しています。
MiniCoreを既にインストール済みなら、手順2でバージョンを確認してください。

**未書き込みのMCUは、最初の1回だけISPで「ブートローダを書き込む」が必要です。**
ISPのみで使う場合は「Upload method」で `ISP (no bootloader)` を選び、スケッチの「書込装置を使って書き込む」を使います。

USB書き込みを使う場合、ツールの「Upload method」は `USB serial (UART0 / Urboot)`、書込装置は実際に使う `USBasp` または `Arduino as ISP` を選択します。
ISPには **SPI0（PB3=MOSI、PB4=MISO、PB5=SCK）・RESET・電源・GND** を使用します。D8～D10のSPI1とは異なります。
Urboot導入後はUSBシリアルから書き込めます。既存の旧Optibootはこの既定設定の対象外です。

## ピン番号

`digitalWrite(4, HIGH)` と `digitalWrite(D4, HIGH)` は同じD4端子を操作します。
番号はATmegaやArduino Unoの番号ではありません。

| XIAO端子 / 数値 | MCU | 主な機能 | ADC別名 | PWM |
|---|---|---|---|---|
| D0 / 0 | PE2 | SPI1 SS | A0 | — |
| D1 / 1 | PD2 | INT0 | — | Timer4B |
| D2 / 2 | PD3 | INT1 | — | Timer2B |
| D3 / 3 | PB1 | GPIO | — | Timer1A |
| D4 / 4 | PC4 | Wire SDA | A4 | — |
| D5 / 5 | PC5 | Wire SCL | A5 | — |
| D6 / 6 | PB3 | Serial1 TX / SPI0 MOSI | — | Timer2A |
| D7 / 7 | PB4 | Serial1 RX / SPI0 MISO | — | — |
| D8 / 8 | PC1 | SPI SCK (SPI1) | A8 | — |
| D9 / 9 | PC0 | SPI MISO (SPI1) | A9 | — |
| D10 / 10 | PE3 | SPI MOSI (SPI1) | A10 | — |

`analogRead(A0)` / `analogRead(D0)` / `analogRead(0)` はすべてPE2のADC6を読みます。
数値をADCチャンネル番号として解釈しません。`PIN_ADC0`～`PIN_ADC7` で物理ADCチャンネルからGPIO番号を得られます。
**D1～D3にはADCがないため、A1～A3は定義していません。** XIAO全機種の機能互換ではありません。
ADCを持たないピンを`analogRead()`に渡さないでください。

`PIN_PB3`など、MCUポート名のマクロも利用できます。
11～26は内部ピン用です（全割り当ては `avr/variants/xiao328pb/pins_arduino.h`）。
PD0/PD1はUSB UART、PD5はユーザーLED、PB6/PB7は発振器、PC6はRESETに使用します。
`LED_BUILTIN`は13（PD5）、`LED_BUILTIN_ACTIVE`は`LOW`です。
この基板のMCU/GPIOは5V系です。XIAOの形状・番号の互換性は3.3V電気特性の互換性を意味しません。

## 通信と特殊ピンのマクロ

```cpp
#include <Wire.h>
#include <SPI.h>

void setup() {
  Serial.begin(115200);  // USB FT232 / UART0
  Serial1.begin(9600);   // D6=TX, D7=RX / UART1
  Wire.begin();         // D4=SDA, D5=SCL / I2C0
  SPI.begin();          // D8=SCK, D9=MISO, D10=MOSI / SPI1
}
void loop() {}
```

| 対象 | マクロ |
|---|---|
| I²C0 | `PIN_WIRE0_SDA`、`PIN_WIRE0_SCL` |
| I²C1 | `PIN_WIRE1_SDA`、`PIN_WIRE1_SCL`（PE0/PE1、ヘッダに出ていません） |
| UART0 | `PIN_SERIAL0_RX`、`PIN_SERIAL0_TX`（USB通信） |
| UART1 | `PIN_SERIAL1_RX`、`PIN_SERIAL1_TX`（D7/D6） |
| 既定SPI | `PIN_SPI_SS`、`PIN_SPI_MOSI`、`PIN_SPI_MISO`、`PIN_SPI_SCK`（SPI1） |
| 物理SPI0/1 | `PIN_SPI0_*`、`PIN_SPI1_*` |

MiniCore形式の `PIN_WIRE_SDA0` / `PIN_SPI_MOSI1` などの別名、`SDA`、`SCL`、`SS`、`MOSI`、`MISO`、`SCK` も使用できます。
`SERIAL_PORT_MONITOR`は`Serial`、`SERIAL_PORT_HARDWARE_OPEN`は`Serial1`です。

`SPI.h`は小さなアダプターで、**別途インストールしたMiniCoreのSPI1実装**を使用します。
`SPI` / `SPIClass` / `SPISettings` と `SPI1` / `SPI1Class` / `SPI1Settings` は同じバスを扱います。
`SPI.begin()`はハードウェアSSのD0も出力にするため、SPI使用中のD0はADC入力と兼用できません。
他の端子をデバイス用CSに使う場合もD0の出力設定が必要です。
レジスタを直接操作するライブラリは別途対応が必要です（MiniCore互換の`SPCR`などはSPI0を指します）。

`Wire1`はMiniCoreの`Wire1.h`で利用できますが、PE0/PE1はこの基板では未接続です。
SPI0とSerial1はPB3/PB4を共有します。マクロの存在だけで機能を同時利用できるわけではありません。

## MiniCoreとの関係

- `upstream/MiniCore` は公式リポジトリへの **Git submodule**。検証対象をv3.1.3のcommitに固定しています。
- `build.core=MiniCore:MCUdude_corefiles` で、Arduino環境にインストール済みのcoreを参照します。
- ビルド手順・書込ツール・ブートローダ・Wire・SPI1等もインストール済みMiniCoreから参照します。
- 配布アーカイブにcore、MiniCore本体、ブートローダ、ツールチェーンを含めません。
- submoduleの固定は開発・検証用です。Arduinoのcore参照自体はバージョン制約を指定できないため、IDE側でも3.1.3を選んでください。
- Arduinoのパッケージ仕様にはplatform間の自動インストール依存関係がないため、MiniCoreを先にインストールします。

参考: [Arduino core/tool reference](https://docs.arduino.cc/arduino-cli/platform-specification/#referencing-another-core-variant-or-tool)、[公式MiniCore](https://github.com/MCUdude/MiniCore)。

## 開発・リリース

```sh
git clone --recurse-submodules https://github.com/PenguinEino/arduino-xiao-atmega328pb.git
cd arduino-xiao-atmega328pb
python3 scripts/package.py
```

`dist/`にvariantのみのアーカイブと統合indexを生成します。ビルドはPython標準ライブラリのみで再現できます。
リリース時はアーカイブをGitHub Releaseへアップロードし、生成indexをリポジトリ直下へ反映します。
新バージョンでは`avr/platform.txt`・サンプルライブラリのバージョンを更新し、`--version`で指定してください。
既存のリリースアーカイブは差し替えず、新しいバージョンを公開します。

```sh
URL=https://raw.githubusercontent.com/PenguinEino/arduino-xiao-atmega328pb/main/package_atmegagokan_index.json
arduino-cli core update-index --additional-urls "$URL"
arduino-cli core install MiniCore:avr@3.1.3 --additional-urls "$URL"
arduino-cli core install atmegagokan:avr --additional-urls "$URL"
arduino-cli compile --fqbn atmegagokan:avr:xiao328pb tests/PinAudit
```

`tests/PinAudit`はコンパイル・ピン定義検証用です。未接続ピンや複数周辺回路を扱うので実機テストとして実行しないでください。
`scripts/check_elf.py`はコンパイル済みELF内の27ピンのテーブルを調べ、回路図由来の`docs/hardware-pinmap.json`と11本のヘッダ信号を照合します。
GitHub Actionsでも別途インストールしたMiniCoreを使ってコンパイルします。
実機での書き込み・通信・ADC・PWM動作は未検証です。

## ライセンス

このvariantおよびSPIアダプターはLGPL-2.1-or-later。MiniCore本体は別リポジトリにあり、各ファイルのライセンスに従います。
