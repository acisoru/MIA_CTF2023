# Camel calc
## Сложность
easy

## Описание
Сегодня с одноклассником листали TikTok и наткнулись на трек "Camel by camel" Sandy Marton'а. Нам понравилось, и мы зашли полистать видео под этим звуком. Внимание привлек тренд про какой-то калькулятор верблюдов. Оказалось, что если ввести свои параметры туда, можно узнать, сколько верблюдов ты стоишь. Держи ссылку, тоже побалуйся с этим сервисом.

## Решение:
По условию, у нас есть только ссылка на какой-то сервис по верблюдам. Откроем его и действительно увидим графы, в которых нужно выбрать свои параметры.

![](https://i.imgur.com/mweMFhA.png)

При вводе обычных значений, мы увидим такой результат.

![](https://i.imgur.com/SCAnkei.png)

Что-то подсказывает, что форма здесь находиться не просто так. Поэтому попробуем узнать, что конкретно можно сделать

Открываем Wappalyzer и сразу видим, что здесь используется Flask. Теперь все сходиться, кажется у нас Jinja Template SSTI injection.

Додуматься до этого шага, можно было и через обычный перебор пэйлоадов, как к примеру на этом сайте - **https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Server%20Side%20Template%20Injection**

![](https://i.imgur.com/K1cBRiM.png)

Насчет перебора пэйлоадов, после отработки самого стандартного **`{{7*7}}`**, стоит задуматься и о других. Ведь цель у нас - найти флаг. 
Полистав множетсво разных, натыкаемся на что-то такое:
**`{{get_flashed_messages.globals.builtins.open("flag.txt").read()}}`**

![](https://i.imgur.com/5rm0mXE.png)
![](https://i.imgur.com/3x12BzS.png)

Пэйлоад и вправду отработал, теперь осталось понять, чем его задекодили. Понять это, нам помогает знак равенства на конце.

Используем любой инструмент декода или magic тулзу и получаем финальный флаг, который уже ждет, когда его загрузят.

![](https://i.imgur.com/XBoqSVA.png)



## Флаг
CTF{cAAAm3l_b7_cAm31_2222}
