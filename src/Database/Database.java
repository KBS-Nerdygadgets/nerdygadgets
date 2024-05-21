package Database;
import java.sql.*;

public class Database {
    public static void main(String[] args) {
        // JDBC Connection Parameters
        String url = "jdbc:mysql://localhost:3306/nerdygadgets";
        String username = "root";
        String password = "";

        // SQL query
        String query = "SELECT * FROM warehouse";

        try {
            // Establish connection
            Connection connection = DriverManager.getConnection(url, username, password);

            // Create statement
            Statement statement = connection.createStatement();

            // Execute query
            ResultSet resultSet = statement.executeQuery(query);

            // Process results
            while (resultSet.next()) {
                // Retrieve data from the result set
                int id = resultSet.getInt("OrderID");
                int X = resultSet.getInt("X");
                int Y = resultSet.getInt("Y");
                // Retrieve other columns as needed

                // Process data
                System.out.println("ID: " + id + ", X-as: " + X + ", Y-as: " + Y);
            }

            // Exit resources
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
