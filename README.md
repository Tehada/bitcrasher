## Bitcrasher 

Алгоритм для отделения текста от фона на `pgm` изображениях.

### Установка

Для работы потребуется [opencv][opencv]. Инструкция по установке библиотеки для [ubuntu][opencv ubuntu] и [os x][opencv os x]. После установки opencv достаточно запустить Makefile, который создаст исполняемый файл `bitcrasher`.

### Использование

`./bitcrasher input.pgm output.pbm` Регулировка параметров будет добавлена в ближайшее время.

### Принцип работы Bitcrasher

Фон чаще всего можно представить в виде комбинации линейных функций, которыми нельзя описать пиксели текста. 

В основе лежит идея поиска линейных функций, которые смогут приближать фон. Рассматриваются блоки пикселей и оценивается их возможность принадлежать фону. Для этого используется [метод наименьших моментов][LAD], который подбирает модель для блока пикселей.

### ADMM

Реализация ADMM была взята [отсюда][ADMM]

### Описание

Алгоритм предназначен для отделения текста от гладкого фона. Предлагается использовать два базовых подхода. Оба основаны на том факте, что гладкий фон в каждом блоке можно представить линейной комбинацией небольшого количества базисных функций, в то время как текст образует резкие разрывы. Алгоритм в каждом блоке разделяет пиксели на фон и текст, пытаясь смоделировать фон гладкой функцией и использует для этого две техники. Первая основана на устойчивой регресси (ссылка), где внутренние пиксели относятся к фону, а всё остальное к тексту. Второй подход известнем как sparse decomposition суть его заключается в моделиовании фонового слоя с помощью гладких и разросанных компонент. Алгоритм предлагается использовать для улучшения степени сжатия сканированных изображений. Bitcrasher является улучшенным аналогом утилиты cpaldjvu, которая плохо справляется с неодноцветным фоном (может появиться на сканированном изображении в месте перегиба страницы, например).

### Сравнение с аналогичными алгоритмами:

В левом верхнем углу оригинальное изображение, далее по часовой стрелке: djvulibre, bitcrasher, simpledjvu.

![alt tag](https://github.com/Tehada/Bitcrasher/blob/master/images/compare/1.jpg)

В [image][first] собраны все тестовые результаты. Директория [original][second] содержит исходные изображения в формате pgm. В директории [djvulibre][third] результат работы утилиты [cpaldjvu][fourth] в формате pbm. В том же формате хранятся результаты работы simpledjvu и bitcrasher в [simpledjvu][fifth] и [bitcrasher][sixth]. В [compare][seventh] содержатся склеенные изображения пониженного качества, но небольшого размера.

[opencv]: http://opencv.org
[opencv ubuntu]: http://www.pyimagesearch.com/2016/10/24/ubuntu-16-04-how-to-install-opencv/
[opencv os x]: http://www.pyimagesearch.com/2015/06/15/install-opencv-3-0-and-python-2-7-on-osx/









------------ Comments (A.S.) ---------

Конечно, в общем виде такой план достаточно понятен, но если обсуждать подробности, то хорошо бы как-то более подробно описать планы (например, чтобы координировать действия с коллегами). Действительно, использовать имеющуюся тестовую базу правильно, но хорошо бы иметь возможность пробовать разные алгоритмы и разные данные, чтобы получить какие-то более разносторонние оценки. Но, может быть, действительно это уже второй этап, на первом хотелось бы хоть что-то попробовать и посмотреть самому, чтобы зря с самого начала не расстраиваться проигрышами :-)

В общем, мне кажется, что итогом первого этапа могли бы быть

1) собственные алгоритмы с помощью нейронной сети
2) набор изображений разного качества и достаточно разнообразных, не только из DEBCO, но и вообще (потому что, скажем, печатные тексты сильно отличаются от ку
3) обзор, какие алгоритмы существуют и
4) сравнительные результаты тестирования.

Вопрос о встраивании этого в djvulibre и вообще тогда можно пока (ненадолго :-) отложить...

Программы Межирова: https://gitlab.com/alih/turbinarize.git

https://jwilk.net/software/didjvu 

minidjvu (sourceforge)

Польские коллеги:

http://bc.klf.uw.edu.pl/298/6/JSBatal_Library-12s.pdf
https://jwilk.net/software/didjvu и др.

----

Возможные альтернативы:

* Улучшения k-means clustering, который встроен в djvulibre

* expectation-maximization algorithm -- 

* k-nearest neighbor classifier -- machine learning algorithm (Rocchio algorythm).



[LAD]: https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%BD%D0%B0%D0%B8%D0%BC%D0%B5%D0%BD%D1%8C%D1%88%D0%B8%D1%85_%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D0%B5%D0%B9

[FGKA]: https://www.ncbi.nlm.nih.gov/pmc/articles/PMC543472/


[first]: https://github.com/Tehada/Bitcrasher/tree/master/images
[second]: https://github.com/Tehada/Bitcrasher/tree/master/images/original
[third]: https://github.com/Tehada/Bitcrasher/tree/master/images/djvulibre
[fourth]: http://djvu.sourceforge.net/doc/man/cpaldjvu.html
[fifth]: https://github.com/Tehada/Bitcrasher/tree/master/images/simpledjvu
[sixth]: https://github.com/Tehada/Bitcrasher/tree/master/images/bitcrasher
[seventh]: https://github.com/Tehada/Bitcrasher/tree/master/images/compare


[ADMM]: https://web.stanford.edu/~boyd/papers/admm