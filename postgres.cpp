#include <iostream>
#include <pqxx/pqxx>

int main() {
    try {
        // Connect to the database
        pqxx::connection conn("dbname=my_next_app user=sunny password=sunny host=localhost port=5432");
        
        if (conn.is_open()) {
            std::cout << "✅ Connected to PostgreSQL successfully!" << std::endl;
            
            // Create a transaction
            pqxx::work txn(conn);
            
            // Execute a simple query
            auto result = txn.exec("SELECT version()");
            
            // Print the result
            for (auto row: result) {
                std::cout << "PostgreSQL version: " << row[0].as<std::string>() << std::endl;
            }
            
            txn.commit();
        } else {
            std::cerr << "❌ Failed to connect to database" << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}