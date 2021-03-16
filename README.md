![Screen](https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/start.png)
________________________________________________________________

#### <p align=center> "Наконец я понял, почему url начинается с HTTP"</p> ####
________________________________________________________________

<details>
<summary>Описание задачи</summary>

________________________________________________________________

<p>Написать HTTP сервер по стандарту RFC 7230-7235 на С++, реализовав:</p>
<p>1. Соединение клиент-сервер;</p>
<p>2. Запуск двух и более серверов;</p>
<p>3. Методы GET, POST, PUT, HEAD;</p>
<p>4. Коды состояний, обработка ошибок;</p>
<p>5. Заголовки запроса/ответа</p>
<p>6. Принятие chunked запроса</p>
<p>7. Автоиндексация</p>
<p>8. Поддержку CGI скриптов</p>
<p>9. Авторизацию клиентов</p>
<p>10. Настраиваемый конфигурационный файл</p>
<p>За основу было взято оригинальное поведение <b>Nginx</b>.</p>

________________________________________________________________
</details>

<details>
<summary>Настройка конфигурационного файла</summary>

________________________________________________________________


<p><b>В основной структуре сервера мы можем указать:</b></p>
<p>1. port - порт сервера;</p>
<p><sub>Пример: "port 8080"</sub></p>
<p>2. server_name - наименование сервера;</p>
<p><sub>Пример: "server_name example.com"</sub></p>
<p>3. ip - ip сервера;</p>
<p><sub>Пример: "ip 127.0.0.1"</sub></p>
<p>4. root - корневая директория;</p>
<p><sub>Пример: "root /Users/atomatoe/Desktop/webserv"</sub></p>
<p>5. error_page - путь до файла, который будет отображаться вместо страницы ошибки;</p>
<p><sub>Пример: "error_page 404 /Users/atomatoe/Desktop/webserv/PagesHtml/test_error_page.html"</sub></p>
<p>6. autoindex - on/off автоиндексацию;</p>
<p><sub>Пример: "autoindex off"</sub></p>

________________________________________________________________

<p><b>В структуре location мы можем указать:</b></p>
<p>1. root - корневая директория;</p>
<p><sub>Пример: "root /Users/atomatoe/Desktop/webserv/YoupiBanane"</sub></p>
<p>2. index - отображаемая страница;</p>
<p><sub>Пример: "index index.html"</sub></p>
<p>3. allow_methods - разрешенные методы у location;</p>
<p><sub>Пример: "allow_methods GET"</sub></p>
<p>4. limit_client_body_size - лимит на посылаемое body от клиента;</p>
<p><sub>Пример: "limit_client_body_size 10"</sub></p>
<p>5. cgi_pass - путь до исполняемого CGI скрипта;</p>
<p><sub>Пример: "cgi_pass .bla /Users/atomatoe/Desktop/webserv/TestingCGI/cgi-bin/cgi_tester"</sub></p>
<p>Так же данный пункт поддерживает опционально возможность добавления пути до интерпретатора для запуска CGI скрипта</p>
<p><sub>Пример: "cgi_pass .php /usr/bin/php /Users/atomatoe/Desktop/webserv/TestingCGI/cgi-bin/cgi_tester"</sub></p>
<p>6. limit_client_body_size - заголовок авторизации;</p>
<p><sub>Пример: "auth_clients /Users/atomatoe/Desktop/webserv/AuthClients/auth_clients"</sub></p>
<p>Авторизация в данном проекте сделана посредством Basic-авторизации. Логин:пароль авторизованных клиентов хранится в файле.</p>

________________________________________________________________

<p><sub>(!) Программа принимает конфигурационный файл в качестве аргумента. (!)</sub></p>
</details>


<details>
<summary>Screenshots</summary>

________________________________________________________________

<details>
<summary>Главная страница</summary>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/page_1.png" alt="Ссылка"></p>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/page_2.png" alt="Ссылка"></p>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/page_3.png" alt="Ссылка"></p>
</details>

<details>
<summary>Ошибки</summary>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/method_not_allowed.png" alt="Ссылка"></p>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/not_found.png" alt="Ссылка"></p>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/not_found2.png" alt="Ссылка"></p>
</details>

<details>
<summary>Терминал, автоиндекс и сеть</summary>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/terminal.png" alt="Ссылка"></p>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/autoindex.png" alt="Ссылка"></p>
<p><img src="https://raw.githubusercontent.com/atomatoe/webserv/master/Screenshots/network.png" alt="Ссылка"></p>
</details>

________________________________________________________________

</details>

<details>
<summary>Ссылки</summary>
<p></p>
<p><a href="https://github.com/atomatoe">Мой github</a></p>
<p><a href="https://github.com/цelease">github Welease</a></p>
<p><a href="https://github.com/timurmug">github Qtamaril</a></p>
<p><a href="https://tools.ietf.org/html/rfc7230">RFC 7230</a></p>
<p><a href="http://www.stolyarov.info/books/programming_intro/vol3">А. В. Столяров "Системы и сети"</a></p>
<p><a href="https://developer.mozilla.org/ru/docs/Web/HTTP/Headers">HTTP заголовки</a></p>
<p><a href="https://developer.mozilla.org/ru/docs/Web/HTTP/Status">HTTP коды состояний</a></p>

</details>


Проект написан по стандарту Си++ 98.
