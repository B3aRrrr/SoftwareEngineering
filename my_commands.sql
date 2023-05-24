DELETE FROM `Message`;

DELETE FROM 'Page';

DELETE FROM `User`;


INSERT INTO `User`
(login, password, first_name, last_name, email)
VALUES('catica', 'B3ar', 'Dmitry', 'Chernyshev', 'noemail@gmail.com');

INSERT INTO `User`
(login, password, first_name, last_name, email)
VALUES('pro100vova', 'vov4ik', 'Vladimir', 'Anisimov', 'noemaileither@gmail.com');

INSERT INTO `User`
(login, password, first_name, last_name, email)
VALUES('pro100kira', 'kirrril', 'Kirrill', 'Kirrillov', 'kiri9@mail.ru');

INSERT INTO `User`
(login, password, first_name, last_name, email)
VALUES('pro100anya', 'anya123', 'Anya', 'Ivanovna', 'chikchirik@mail.ru');

INSERT INTO `User`
(login, password, first_name, last_name, email)
VALUES('nepro100ivan', 'ivan4k', 'Ivan', 'Ivanov', 'ivanich@gmail.com');




INSERT INTO "Page"
(first_name, last_name, lovely_books, lovely_musics,lovely_movies,religion)
VALUES('Ivan', 'Ivanov', 'Harry Potter p7', "Mozart","Interstellar","Buddist");
INSERT INTO "Page"
(first_name, last_name, lovely_books, lovely_musics,lovely_movies,religion)
VALUES('Anya', 'Ivanovna', 'Harry Potter p6', "Bach","Interstellar 2"," ne Buddist");
INSERT INTO "Page"
(first_name, last_name, lovely_books, lovely_musics,lovely_movies,religion)
VALUES('Kirrill', 'Kirrillov', 'Harry Potter p5', "No music","Interstellar 3","Buddist");

INSERT INTO "Page"
(first_name, last_name, lovely_books, lovely_musics,lovely_movies,religion)
VALUES('Vladimir', 'Anisimov', 'Harry Potter p4', "Eminem","Interstellar Back","Buddist");
INSERT INTO "Page"
(first_name, last_name, lovely_books, lovely_musics,lovely_movies,religion)
VALUES('Dmitry', 'Chernyshev', 'Harry Potter p1', "Eminem","Interstellar Strikes Back","Buddist");	   


INSERT INTO 'Message'
(id_from, id_to, `message`, 'date')
VALUES((select id from `User` where login = 'nepro100ivan'), 
	   (select id from `User` where login = 'pro100anya'), 'I hate Linux', NOW());