import java.sql.*;

public class Database {
    public static void main(String[] args) {
        // JDBC connection parameters
        String url = "jdbc:mysql://localhost:3306/nerdygadgets";
        String username = "root";
        String password = "";

        // SQL query
        String query = "SELECT * FROM orderlines WHERE OrderLineID = 1";

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
                String description = resultSet.getString("Description");
                // Retrieve other columns as needed

                // Process data
                System.out.println("ID: " + id + ", Description: " + description);
            }

            // Close resources
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
