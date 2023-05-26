#include "page.h"
#include "../database.h"
#include "../../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Page::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            for (auto &hint : database::Database::get_all_hints())
            {
                Statement create_stmt(session);
                create_stmt << "CREATE TABLE IF NOT EXISTS `Page` (`id` INT NOT NULL AUTO_INCREMENT,"
                            << "`first_name` VARCHAR(256) NOT NULL,"
                            << "`last_name` VARCHAR(256) NOT NULL,"       
                            << "`lovely_books` VARCHAR(256) NOT NULL,"
                            << "`lovely_musics` VARCHAR(256) NOT NULL,"
                            << "`lovely_movies` VARCHAR(256) NULL,"
                            << "`religion` VARCHAR(256) NULL,"
                            << "`email` VARCHAR(256) NOT NULL,"

                            << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));"
                            << hint,
                            now;

                std::cout << create_stmt.toString() << std::endl;
            }
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        } 
    }

    Poco::JSON::Object::Ptr Page::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("lovely_books", _lovely_books);
        root->set("lovely_musics", _lovely_musics);
        root->set("lovely_movies", _lovely_movies);
        root->set("religion", _religion);
        root->set("email", _email);

        return root;
    }
    Page Page::fromJSON(const std::string &str)
    {
        Page page;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        page.id() = object->getValue<long>("id");
        page.first_name() = object->getValue<std::string>("first_name");
        page.last_name() = object->getValue<std::string>("last_name");
        page.lovely_books() = object->getValue<std::string>("lovely_books");
        page.lovely_musics() = object->getValue<std::string>("lovely_musics");
        page.lovely_movies() = object->getValue<std::string>("lovely_movies");
        page.religion() = object->getValue<std::string>("religion");
        page.email() = object->getValue<std::string>("email");

        return page;
    }
     

    long Page::get_id() const
    {
        return _id;
    }

    const std::string &Page::get_first_name() const
    {
        return _first_name;
    }
    const std::string &Page::get_last_name() const
    {
        return _last_name;
    }
    const std::string &Page::get_books() const
    {
        return _lovely_books;
    }
    const std::string &Page::get_musics() const
    {
        return _lovely_musics;
    }
    const std::string &Page::get_movies() const
    {
        return _lovely_movies;
    }
    const std::string &Page::get_religion() const
    {
        return _religion;
    }
    const std::string &Page::get_email() const
    {
        return _email;
    }
    
    long &Page::id()
    {
        return _id;
    }
    std::string &Page::first_name()
    {
        return _first_name;
    }
    std::string &Page::last_name()
    {
        return _last_name;
    }
    std::string &Page::lovely_books()
    {
        return _lovely_books;
    }
    std::string &Page::lovely_musics()
    {
        return _lovely_musics;
    }
    std::string &Page::lovely_movies()
    {
        return _lovely_movies;
    }
    std::string &Page::religion()
    {
        return _religion;
    }
    std::string &Page::email()
    {
        return _email;
    }
 
    std::vector<Page> Page::load_info(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::vector<Page> result;
            Page a;
            std::string sharding_hint = database::Database::sharding_hint_2(first_name,last_name);
            std::string select_str = "SELECT id, first_name, last_name, email, lovely_books,lovely_musics, lovely_movies, religion FROM Page where first_name=? and last_name=?";
            select_str += sharding_hint;
            std::cout << select_str << std::endl;
            
            first_name += "%";
            last_name += "%";
            select << select_str,
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._lovely_books),
                into(a._lovely_musics),
                into(a._lovely_movies),
                into(a._religion),
                use(first_name),
                use(last_name),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }
    

    void Page::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            std::string sharding_hint = database::Database::sharding_hint_2(_first_name,_last_name);
            std::string insert_str = "INSERT INTO Page (first_name,last_name,email,lovely_books,lovely_musics,lovely_movies,religion) VALUES(?,?,?,?,?,?,?)";
            insert_str += sharding_hint;
            std::cout << insert_str << std::endl;

            insert << insert_str,
                use(_first_name),
                use(_last_name),
                use(_email),
                use(_lovely_books),
                use(_lovely_musics),
                use(_lovely_movies);
                use(_religion);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()"+sharding_hint;
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

}