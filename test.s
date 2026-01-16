# full_program.s - программа с циклом, условием, массивом и сложением
.text
.global _start

_start:
    # 1. Выделяем память для массива из 10 элементов на стеке
    addi sp, sp, -40           # 10 слов * 4 байта = 40 байт
    
    # 2. Заполнение массива (цикл)
    li t0, 0                   # i = 0 (счетчик)
    li t1, 10                  # size = 10
    mv t2, sp                  # адрес начала массива
    
fill_loop:
    bge t0, t1, end_fill       # if i >= size, выходим
    
    # array[i] = i + 1
    addi t3, t0, 1            # value = i + 1
    slli t4, t0, 2            # offset = i * 4
    add t5, t2, t4            # addr = array + offset
    sw t3, 0(t5)              # сохраняем значение
    
    addi t0, t0, 1            # i++
    j fill_loop

end_fill:
    # 3. Суммирование элементов массива (второй цикл)
    li t0, 0                  # i = 0
    li t6, 0                  # sum = 0
    
sum_loop:
    bge t0, t1, check_condition  # if i >= size, выходим
    
    # Загружаем array[i]
    slli t4, t0, 2            # offset = i * 4
    add t5, t2, t4            # addr = array + offset
    lw t3, 0(t5)              # t3 = array[i]
    
    add t6, t6, t3            # sum += array[i]
    addi t0, t0, 1            # i++
    j sum_loop

check_condition:
    # 4. Условный оператор if
    li t3, 15                 # threshold = 15
    
    # if (sum > 15)
    ble t6, t3, else_part     # if sum <= 15, переходим в else
    
    # then-часть
    li a0, 1                  # результат = 1
    j cleanup
    
else_part:
    # else-часть
    li a0, 0                  # результат = 0

cleanup:
    # Освобождаем память
    addi sp, sp, 40
    
    # Завершение программы
    li a7, 93                 # syscall exit
    ecall
