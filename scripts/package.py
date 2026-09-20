#!/usr/bin/env python3
"""Create a reproducible variant-only archive and the combined index."""
import argparse, hashlib, io, json, tarfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
ap = argparse.ArgumentParser()
ap.add_argument('--version', default='0.1.0')
ap.add_argument('--repository', default='PenguinEino/arduino-xiao-atmega328pb')
ap.add_argument('--base-url', help='Archive URL prefix for local installation tests')
a = ap.parse_args()
version = a.version
assert f'version={version}\n' in (ROOT/'avr/platform.txt').read_text()
out = ROOT/'dist'; out.mkdir(exist_ok=True)
archive = out/f'atmegagokan-avr-{version}.tar.bz2'
with tarfile.open(archive, 'w:bz2', format=tarfile.USTAR_FORMAT) as tf:
    for p in sorted((ROOT/'avr').rglob('*')):
        if not p.is_file(): continue
        assert not p.is_symlink(), p
        data = p.read_bytes()
        ti = tarfile.TarInfo(f'atmegagokan-avr-{version}/'+str(p.relative_to(ROOT/'avr')))
        ti.size = len(data); ti.mode = 0o644; ti.mtime = 0
        tf.addfile(ti, io.BytesIO(data))
    for name in ('LICENSE', 'README.md'):
        data = (ROOT/name).read_bytes()
        ti = tarfile.TarInfo(f'atmegagokan-avr-{version}/{name}')
        ti.size = len(data); ti.mode = 0o644; ti.mtime = 0
        tf.addfile(ti, io.BytesIO(data))
base = a.base_url or f'https://github.com/{a.repository}/releases/download/v{version}'
platform = dict(name='ATmega Gokan XIAO (install MiniCore first)', architecture='avr', version=version,
    category='Contributed', url=f'{base}/{archive.name}', archiveFileName=archive.name,
    checksum='SHA-256:'+hashlib.sha256(archive.read_bytes()).hexdigest(), size=str(archive.stat().st_size),
    boards=[dict(name='ATmega Gokan XIAO (ATmega328PB, 16 MHz)')], toolsDependencies=[])
package = dict(name='atmegagokan', maintainer='PenguinEino', websiteURL=f'https://github.com/{a.repository}',
    email='', help=dict(online=f'https://github.com/{a.repository}/issues'), platforms=[platform], tools=[])
# Metadata only; MiniCore code/tool archives remain at their official URLs.
index = dict(packages=[package]+json.loads((ROOT/'upstream/minicore-index.json').read_text())['packages'])
# Retain previously published versions when producing an update.
old_index = ROOT/'package_atmegagokan_index.json'
if old_index.exists():
    old = json.loads(old_index.read_text())['packages'][0]['platforms']
    package['platforms'] += [p for p in old if p['version'] != version]
(out/'package_atmegagokan_index.json').write_text(json.dumps(index, indent=2)+'\n')
with tarfile.open(archive) as tf:
    assert not any('/cores/' in n or '/bootloaders/' in n or '/upstream/' in n for n in tf.getnames())
print(archive)
print(platform['checksum'])
