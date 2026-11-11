"""Validate Keil references and CubeMX object counts without running Keil."""
from pathlib import Path
import xml.etree.ElementTree as E
import re
r=Path(__file__).resolve().parent.parent
base=r/'Teris/MDK-ARM';t=E.parse(base/'Teris.uvprojx')
files=[(base/n.text).resolve() for n in t.findall('.//Group//FilePath')]
assert len(files)==len(set(files))
for f in files:assert f.is_file(),f
for n in t.iter('IncludePath'):
 for p in (n.text or '').split(';'):
  if p:assert (base/p).is_dir(),p
for name,start,size in [('IROM','0x08000000','0x10000'),('IRAM','0x20000000','0x5000')]:
 n=t.find('.//OnChipMemories/'+name);assert int(n.findtext('StartAddress'),16)==int(start,16);assert n.findtext('Size')==size
s=(r/'main.c').read_text()
assert len(re.findall(r'\bosThreadNew\s*\(',s))==3
assert len(re.findall(r'\bosMessageQueueNew\s*\(',s))==2
assert len(re.findall(r'\bosKernelStart\s*\(',s))==1
for name in ['input','game','display']:assert 'task_'+name+'_run(argument);' in s
for folder in ['app/src','hal/src']:
 for f in (r/folder).glob('*.c'):assert f.resolve() in files;assert 'HAL_Delay(' not in f.read_text()
print('PASS: Keil source/include/memory references; exactly 3 application Tasks, 2 queues, 1 kernel start')

for folder in ["bsp/inc","bsp/src","hal/inc","hal/src","middlewares/FreeRTOS","app/inc","app/src"]:assert (r/folder).is_dir()

irq=(r/'bsp/src/stm32f1xx_it.c').read_text()
assert len(re.findall(r'void USART1_IRQHandler\s*\(void\)',irq))==1
assert 'uart_debug_irq();' in irq
print('PASS: USART1 handler linked to nonblocking debug driver; no new RTOS objects')
