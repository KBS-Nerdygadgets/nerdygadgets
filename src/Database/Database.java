package Database;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import TSP.Point;

public class Database {
    public static List<Point> fetchPointsFromDatabase() {
        List<Point> points = new ArrayList<>();

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
                int x = resultSet.getInt("X");
                int y = resultSet.getInt("Y");

                // Create a new Point object and add it to the list
                Point point = new Point(x, y);
                points.add(point);
            }

            // Exit resources
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return points;
    }
}
