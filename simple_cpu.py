# configs/riscv/simple_cpu.py
import m5
from m5.objects import *
import sys
import os

# Создание системы
system = System()

# Настройка тактовой частоты
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Процессор
system.cpu = AtomicSimpleCPU()

# Создаем контроллер прерываний для RISC-V
system.cpu.interrupts = [RiscvInterrupts()]

# Память - ВАЖНО: AtomicSimpleCPU требует атомарный режим памяти!
system.mem_mode = 'atomic'  # Изменено с 'timing' на 'atomic'

system.mem_ranges = [AddrRange('512MB')]

# Шина
system.membus = SystemXBar()

# Подключение процессора
system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

# Контроллер памяти
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# Настройка системы
system.system_port = system.membus.cpu_side_ports

# Проверяем существование исполняемого файла
binary = 'test.riscv'
if not os.path.exists(binary):
    print(f"Error: Binary '{binary}' not found in {os.getcwd()}!")
    sys.exit(1)

# Процесс
process = Process()
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

# Устанавливаем рабочую нагрузку системы
system.workload = SEWorkload.init_compatible(binary)

# Корневая система
root = Root(full_system=False, system=system)

print("Starting simulation with modified AtomicSimpleCPU (no MMU)...")
print(f"Running binary: {binary}")
print(f"Memory mode: {system.mem_mode}")

m5.instantiate()

# Запуск симуляции
exit_event = m5.simulate()
print(f'Exiting @ tick {m5.curTick()} because {exit_event.getCause()}')
