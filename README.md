# Социальная сеть
Необходимо написать программную реализацию социальной сет. 

Приложение должно поддерживать работу со следующими данными:

* Страница пользователя
* Обмен сообщениями чат

Требуется реализовать следующий API:

* Создание нового пользователя
* Поиск пользователя по логину
* Поиск пользователя по маске имени и фамилии
* Создание и заполнение страницы пользователя
* Добавление информацмии на страницу
* Запрос для просмотра других страниц из базы данных

***UPD***

Для запуска микросервисов в Docker,  выполните команду в консоли:
```
docker-compose up --build
```
Тестовые данные для загрузки в базу данных находятся в [my_commands.sql](https://github.com/B3aRrrr/SoftwareEngineering/blob/master/my_commands.sql).

Все таблицы базы данных для работы сервисов будут созданы при первом запуске сервисов.

* Сервис авторизации: [services/web_service](https://github.com/B3aRrrr/SoftwareEngineering/tree/master/services/web_service)
* Сервис  чатов: [services/message_service](https://github.com/B3aRrrr/SoftwareEngineering/tree/master/services/message_service)
* Сервис страниц с личной информация: [services/page_service](https://github.com/B3aRrrr/SoftwareEngineering/tree/master/services/page_service)
```
