#!/usr/bin/env python3
"""Build a reproducible package from the pinned MiniCore submodule and overlay."""
import argparse, hashlib, io, json, tarfile, subprocess
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
ap = argparse.ArgumentParser()
ap.add_argument('--version', default=dict(line.split('=', 1) for line in (ROOT/'avr/platform.txt').read_text().splitlines())['version'])
ap.add_argument('--repository', default='PenguinEino/arduino-xiao-atmega328pb')
ap.add_argument('--base-url', help='Archive URL prefix for local installation tests')
a = ap.parse_args()
version = a.version
assert f'version={version}\n' in (ROOT/'avr/platform.txt').read_text()
out = ROOT/'dist'; out.mkdir(exist_ok=True)
archive = out/f'atmegagokan-avr-{version}.tar.bz2'
lock = json.loads((ROOT/'upstream/lock.json').read_text())
upstream = ROOT/'upstream/MiniCore'
commit = subprocess.check_output(['git', '-C', str(upstream), 'rev-parse', 'HEAD'], text=True).strip()
assert commit == lock['commit'], 'MiniCore submodule does not match lock.json'
assert not subprocess.check_output(['git', '-C', str(upstream), 'status', '--porcelain'], text=True).strip(), 'MiniCore submodule is modified'
files = {}
for directory in ('cores', 'libraries', 'bootloaders', 'scripts'):
    for p in sorted((upstream/'avr'/directory).rglob('*')):
        if p.is_file():
            relative = p.relative_to(upstream/'avr').as_posix()
            if relative.startswith('libraries/SPI/'): continue
            assert not p.is_symlink(), p
            files[relative] = p.read_bytes()
# Upstream recipes are assembled at release time, not copied into source control.
files['platform.txt'] = (upstream/'avr/platform.txt').read_bytes()
for p in sorted((ROOT/'avr').rglob('*')):
    if not p.is_file(): continue
    assert not p.is_symlink(), p
    relative = p.relative_to(ROOT/'avr').as_posix()
    if relative == 'platform.txt':
        files[relative] += b'\n# ATmega Gokan overrides\n' + p.read_bytes()
    else:
        files[relative] = p.read_bytes()
for name in ('LICENSE', 'README.md'):
    files[name] = (ROOT/name).read_bytes()
files['MiniCore-LICENSE'] = (upstream/'LICENSE').read_bytes()
files['MiniCore-source.json'] = (ROOT/'upstream/lock.json').read_bytes()
with tarfile.open(archive, 'w:bz2', format=tarfile.PAX_FORMAT) as tf:
    for name, data in sorted(files.items()):
        ti = tarfile.TarInfo(f'atmegagokan-avr-{version}/'+name)
        ti.size = len(data); ti.mode = 0o755 if name.endswith('.sh') else 0o644; ti.mtime = 0
        tf.addfile(ti, io.BytesIO(data))

base = a.base_url or f'https://github.com/{a.repository}/releases/download/v{version}'
mini = json.loads((ROOT/'upstream/minicore-index.json').read_text())['packages'][0]
dependencies = [dict(d, packager='atmegagokan' if d['packager']=='MiniCore' else d['packager']) for d in mini['platforms'][0]['toolsDependencies']]
platform = dict(name='ATmega Gokan XIAO', architecture='avr', version=version,
    category='Contributed', url=f'{base}/{archive.name}', archiveFileName=archive.name,
    checksum='SHA-256:'+hashlib.sha256(archive.read_bytes()).hexdigest(), size=str(archive.stat().st_size),
    boards=[dict(name='ATmega Gokan XIAO (ATmega328PB, 16 MHz)')], toolsDependencies=dependencies)
package = dict(name='atmegagokan', maintainer='PenguinEino', websiteURL=f'https://github.com/{a.repository}',
    email='', help=dict(online=f'https://github.com/{a.repository}/issues'), platforms=[platform], tools=mini['tools'])
# Preserve MiniCore metadata for legacy 0.1.0 installations; 0.2.0 installs independently.
index = dict(packages=[package]+json.loads((ROOT/'upstream/minicore-index.json').read_text())['packages'])
# Retain previously published versions when producing an update.
old_index = ROOT/'package_atmegagokan_index.json'
if old_index.exists():
    old = json.loads(old_index.read_text())['packages'][0]['platforms']
    package['platforms'] += [p for p in old if p['version'] != version]
(out/'package_atmegagokan_index.json').write_text(json.dumps(index, indent=2)+'\n')
with tarfile.open(archive) as tf:
    names = tf.getnames()
    assert any('/cores/MCUdude_corefiles/Arduino.h' in n for n in names)
    assert any('/libraries/SPI1/src/SPI1.cpp' in n for n in names)
    assert not any('/.git/' in n or '/upstream/' in n for n in names)
print(archive)
print(platform['checksum'])
