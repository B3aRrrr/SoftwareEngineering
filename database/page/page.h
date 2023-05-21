#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Page{
        private:
            long _id;
            /*identification*/
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            /*info*/
            std::string _lovely_books;
            std::string _lovely_musics;
            std::string _lovely_movies;
            std::string _religion;

        public:

            static Page fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;

            const std::string &get_books() const;
            const std::string &get_musics() const;
            const std::string &get_movies() const;
            const std::string &get_religion() const;
            const std::string &get_email() const;

            long&        id();
            std::string &first_name();
            std::string &last_name();
            std::string &lovely_books();
            std::string &lovely_musics();
            std::string &lovely_movies();
            std::string &religion();
            std::string &email();

            static void init();
            static std::vector<Page> load_info(std::string first_name, std::string last_name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif