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
            // Connectie maken
            Connection connection = DriverManager.getConnection(url, username, password);

            // Create statement
            Statement statement = connection.createStatement();

            // Execute query
            ResultSet resultSet = statement.executeQuery(query);

            // Resultaten processen
            while (resultSet.next()) {
                // Data ophalen
                int x = resultSet.getInt("X");
                int y = resultSet.getInt("Y");

                // Nieuw punt aanmaken en toevoegen aan de lijst
                Point point = new Point(x, y);
                points.add(point);
            }

            // Afsluiten
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return points;
    }
}
