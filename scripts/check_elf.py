#!/usr/bin/env python3
"""Check compiled AVR pin tables, independent of the variant source generator."""
import argparse, json, subprocess, tempfile
from pathlib import Path
ap=argparse.ArgumentParser(); ap.add_argument('elf'); ap.add_argument('--avr-bin', required=True); a=ap.parse_args()
root=Path(__file__).resolve().parents[1]
symbols={}
for line in subprocess.check_output([str(Path(a.avr_bin)/'avr-nm'), '-S', a.elf],text=True).splitlines():
    cols=line.split()
    if len(cols)==4: symbols[cols[3]]=(int(cols[0],16),int(cols[1],16))
with tempfile.TemporaryDirectory() as d:
    raw=Path(d)/'flash.bin'
    subprocess.run([str(Path(a.avr_bin)/'avr-objcopy'),'-O','binary','-j','.text',a.elf,str(raw)],check=True)
    flash=raw.read_bytes()
def table(name):
    addr,size=symbols[name]; return list(flash[addr:addr+size])
ports=table('digital_pin_to_port_PGM'); masks=table('digital_pin_to_bit_mask_PGM'); timers=table('digital_pin_to_timer_PGM')
assert len(ports)==len(masks)==len(timers)==27
for pin in json.loads((root/'docs/hardware-pinmap.json').read_text())['header']:
    i=pin['digital']; p=pin['port']
    assert ports[i]=={'PB':2,'PC':3,'PD':4,'PE':5}[p[:2]], (i,ports[i])
    assert masks[i]==1<<int(p[2]), (i,masks[i])
assert len(set(zip(ports,masks)))==27, 'Duplicated MCU GPIO'
assert ports[11:]==[4,4,4,3,3,4,4,4,2,2,2,5,5,2,2,3]
assert masks[11:]==[1,2,32,4,8,16,64,128,1,4,32,1,2,64,128,64]
# MiniCore Arduino.h: TIMER4B=14, TIMER2B=9, TIMER1A=4, TIMER2A=8.
assert timers[:11]==[0,14,9,4,0,0,8,0,0,0,0], timers[:11]
print('PASS: 27 compiled GPIO entries; all 11 header nets; PWM timers; unique ports/bits')
