# ANDEME
ANonymous DEcentralized MEssenger (training project)

## FAQ:
- [Как собрать](docs/build.md)

## Зачем 

Данный проект является учебным. Основная цель - ознакомить начинающих программистов с практиками промышленного программирования (codestyle, review, тестирование, ... ), а так же навыки работы в команде.

## Что планируем сделать

Основная цель - сделать распределённый децентрализованный мессенджер.

Каждый пользователь запускает на своём компьютере grpc service, который реализует узел сети.
Каждый узел равноправен и соединяется с несколькими другими узлами.
Узлы будут обмениваться между собой сообщениями по протоколу [GOSSIP](https://en.wikipedia.org/wiki/Gossip_protocol) или аналогичному. В итоге клиентское GUI приложение может отправить сообщение на любой доступный узел сети и в итоге оно окажется на всех узлах сети.

Аутентификация пользователей будет осуществляться за счёт публикации открытого открытого ключа пользователя и его ника. А т.к. все сообщения будут подписываться с использованием закрытого ключа.

В итоге будет гарантироваться, что сообщение пришло от конкретного пользователя, но без трассировки сообщений не будет известно кто именно этот пользователь.


## Стадия 0 - прототип

Для начала работы был сделан минимальный прототип приложения. Он состоит из следующих частей(папок):

1. schema - proto3 схема сообщения и grpc service (MessageSvc). Во время сборки автоматически генерируется C++ код по данным схемам.
В прототипе сообщение (message.proto) имеет только одно поле - text. А MessageSvc - имеет только два метода: Send - отправляющий сообщение на server, и Subscribe - который в ответ отправляет все полученные сообщения.


2. common - общий код для всех приложений. Собирается в статическую библиотеку.

3. client - клиентский API, собирается в статическую библиотеку. Инкапсулирует в себе общение с grpc service, предоставляя высокоуровневый API

4. server - grpc service, узел сети. Наследуется от сгенерированного по схеме grpc MessageSvc service и реализует его бизнес логику.
В прототипе server только складывает полученные сообщения в очередь, и отправляет из очереди сообщения всем подписчикам (клиентам)

5. qtgui - клиентское GUI приложение на Qt5, которое использует client для отправки и получения сообщений от server.

## План работ

Планируется добавить следующие взаимонезависимые функциональности. По мере добавления будем создавать новые релизы.

1. Аутентификация пользователя - для этого надо сделать:
    - добавить в common компонент для работы с private/public ключами
    - дбавить в common компонент для расчёта и проверки цифровой подписи сообщений
    - добавить в GUI диалог для задания имени пользователя
    - добавить в shema новое сообщениe (например user.proto) для рассылки имени пользователя и открытого ключа (с подписью закрытым)
    - добавить в messagesvc.proto метод для получения User сообщения
    - реализовать в server обработку User сообщения, сохранения и рассылки дальше по сети.
    - добавить в client получение информации о user и проверки подписей
    - добавить в GUI отображение информации о user

2. Синхронизация нескольких Node с использованием алгоритма Gossip
    - добавить в common компонент для хранения сообщений в локальной БД (например в sqlite)
    - заиспользовать данный компонент в server для хранения сообщений
    - реализовать алгоритм Gossip для рассылки сообщений по всем узлам (при получении сообщения сервер рассылает его другим узлам)
    - добавить в messagesvc.proto метод для запроса всех сообщений (или только части) с сервера (история сообщений)
    - реализовать в server отправку истории сообщений
    - добавить в server запрос истории сообщений с других узлов при старте.

3. Исследовательские задачи
    - изучить и предложить алгоритм Service Discovery - надо чтобы узлы могли друг друга обнаруживать.
    - изучить и предложить алгоритм подержания связности сети. Т.е. сеть узлов должна быть связанным графом с избыточностью (на случай отключения узлов), но при этом нагрузка на узлы должна быть равномерно распределена.

4. Отказоустойчивость клиента (client)
    - добавить подключение к нескольким серверам и распределения нагрузки между ними
    - переподключение к серверу

5. GUI
    - добавление смайликов





