# Пример первого домашнего задания по курсу Архитектура программных систем

Обращаю внимание что в сервисах нужно реализовтаь схему простой аутентификации (в данном случае) other_service - выполняет роль стороннего сервиса, а сервис hl_mai_lab_01 - сервиса который работает с данными пользователей и осуществляет аутентификацию
```plantuml
@startuml
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml


Person(user, "Пользователь")

Container(web_service, "Сервис работы с пользователем", "C++")    
Container(page_service, "Сервис работы с страницей пользователя", "C++") 
Container(message_service, "Сервис общения пользователей", "C++") 

Rel(user, web_service, "Управлять пользователям")
Rel(page, page_service, "Управлять информацией страниц пользоваетелей")
Rel(message, message_service, "Работа с сообщениями")
Rel(user_service, "/auth - проверка логина/пароля")

@enduml
```
