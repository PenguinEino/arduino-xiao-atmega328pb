#!/usr/bin/env python3
"""Install only our package in a clean Arduino environment and compile it."""
import argparse, functools, http.server, json, subprocess, tempfile, threading
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
ap = argparse.ArgumentParser()
ap.add_argument('--cli', default='arduino-cli')
ap.add_argument('--url', help='Test a published index instead of a local build')
a = ap.parse_args()
version = dict(line.split('=', 1) for line in (ROOT/'avr/platform.txt').read_text().splitlines())['version']
server = None
try:
    if a.url:
        url = a.url
    else:
        handler = functools.partial(http.server.SimpleHTTPRequestHandler, directory=str(ROOT/'dist'))
        server = http.server.ThreadingHTTPServer(('127.0.0.1', 0), handler)
        base = f'http://127.0.0.1:{server.server_port}'
        subprocess.run(['python3', str(ROOT/'scripts/package.py'), '--version', version, '--base-url', base], check=True)
        threading.Thread(target=server.serve_forever, daemon=True).start()
        url = base+'/package_atmegagokan_index.json'
    with tempfile.TemporaryDirectory(prefix='gokan-smoke-') as temp:
        t = Path(temp)
        config = t/'arduino-cli.yaml'
        config.write_text(json.dumps({'directories': {'data': str(t/'data'), 'downloads': str(t/'downloads'), 'user': str(t/'user')}, 'board_manager': {'additional_urls': [url]}}))
        def cli(*args):
            return subprocess.run([a.cli, '--config-file', str(config), *args], check=True)
        cli('core', 'update-index')
        cli('core', 'install', f'atmegagokan:avr@{version}')
        assert not (t/'data/packages/MiniCore/hardware').exists(), 'Separate MiniCore platform unexpectedly installed'
        platform = t/f'data/packages/atmegagokan/hardware/avr/{version}'
        assert (platform/'cores/MCUdude_corefiles/Arduino.h').exists()
        assert (platform/'bootloaders/urboot/atmega328pb/watchdog_1_s/autobaud/uart0_rxd0_txd1/no-led/urboot_atmega328pb_pr_ee_ce.hex').exists()
        for sketch in sorted((ROOT/'avr/libraries/ATmegaGokan/examples').iterdir()):
            cli('compile', '--fqbn', 'atmegagokan:avr:xiao328pb', str(sketch))
        build = t/'audit'
        cli('compile', '--fqbn', 'atmegagokan:avr:xiao328pb', '--build-path', str(build), str(ROOT/'tests/PinAudit'))
        avr_bin = next((t/'data/packages/arduino/tools/avr-gcc').glob('*/bin'))
        subprocess.run(['python3', str(ROOT/'scripts/check_elf.py'), str(build/'PinAudit.ino.elf'), '--avr-bin', str(avr_bin)], check=True)
        cli('compile', '--fqbn', 'atmegagokan:avr:xiao328pb:bootloader=isp,LTO=disabled', str(ROOT/'tests/PinAudit'))
        print('PASS: single-package installation, examples, ELF pin audit and ISP build')
finally:
    if server:
        server.shutdown()
        subprocess.run(['python3', str(ROOT/'scripts/package.py'), '--version', version], check=True)
