# 0.3.0 検証内容

- A0～A5をD0/D4/D5/D8/D9/D10の順へ変更。A6/A7は内部PC2/PC3。
- PinAuditでA番号→D番号→物理ADCチャンネルと、数値4とA4の違いを静的検証。
- analogInputToDigitalPinの論理A番号変換、PIN_ADCxの物理チャンネル指定を検証。
- D番号・通信ピン・27 GPIOテーブルは変更なし。
- 公開前にActionsで単独インストール、サンプル、ELF照合、ISP設定を検証。
- 実機でのADC動作は未検証。

# 0.2.0 検証記録

- submoduleからMiniCoreを取り込んだ配布物を、空のArduino環境へインストール。
- `core install atmegagokan:avr@0.2.0` のみ実行し、MiniCore platformが別途インストールされていないことを確認。
- コンパイラ・avrdude等が依存ツールとして自動取得されることを確認。
- Blink、PinMacros、SerialBridge、PinAudit、ISP/LTO無効設定のコンパイルを検証。
- ELF内の27 GPIO、11本のヘッダ接続、PWMテーブルを検証。
- Actionsは同じ単独インストール検証に成功してからReleaseと登録用JSONを公開。
- 実機動作は未検証。

以下は旧版の記録です。

# 0.1.0 検証記録

- Arduino IDE付属CLI 1.4.1、macOS arm64で検証。
- 既存のArduino設定と分離したdata/userディレクトリを使用。
- MiniCore 3.1.3を公式配布から別途インストール。
- 現行QFN回路図のネットリストと、現行PCBのU3/U6パッドでD0〜D10の11接続を照合。
- Blink、SerialBridge、PinMacrosのコンパイル成功。
- PinAuditでUART0/1、Wire/Wire1、SPI/SPI1、SoftwareSerial、EEPROM、外部割り込み、PWM、ADC別名をビルド。
- USBシリアル既定設定と、ISP・LTO無効設定の両方をビルド。
- ELF内の27 GPIOのポート・ビット・タイマーテーブルを検証。重複なし、11本のヘッダネット一致。
- HTTP配信したindexをボードマネージャ経由でインストール。開発用hardwareリンクを取り除いて検証。
- MiniCoreのcore/platform/ツールを外部参照し、独自variantとSPIアダプターが選択されることを確認。
- 配布アーカイブにcores/bootloaders/upstreamが入っていないことを検査。

実機への書き込み、UART/I2C/SPI通信、ADC精度、PWM波形、ブートローダ焼き込みは未検証。
PinAuditはコンパイル用であり、そのまま実機で実行するためのスケッチではありません。

## 公開後の確認

- GitHub Release v0.1.0のアーカイブ: 15,281 bytes。
- SHA-256: `90d536f912e30a99f58c52d8029b8d16bcab70f6178c89dc04fe8e3484fa67a2`。
- 別の空のdata/userディレクトリで公開index URLのみを追加し、MiniCore 3.1.3とatmegagokan 0.1.0のインストールに成功。
- 公開パッケージによるPinAuditコンパイル・ELF検証成功（8,564 bytes flash / 761 bytes SRAM）。
- 展開済み設定からMiniCore core、独自variant、MiniCoreのUART0 Urbootファイルの実在を確認。
- [Linux CI成功](https://github.com/PenguinEino/arduino-xiao-atmega328pb/actions/runs/35518067569)。
