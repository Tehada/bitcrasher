# Алгоритм преобразования изображений в djvu

Предлагается использовать свёрточную нейронную сеть на основе Tensorflow для бинаризации изображений. На данном этапе устройство сети довольно примитивно: просматривается каждый пиксель по отдельности, затем делается предположение о его цвете. Следующий шаг - научиться учитывать локальную информацию, чтобы окружающие пиксели влияли на итоговое решение.

## План:

1. алгоритм бинаризации
2. создание общего словаря символов
3. усовершенствование алгоритма