package TSP;

import java.util.ArrayList;
import java.util.List;
import Database.Database;

public class Main {
    public static void main(String[] args) {
        // Fetch points from the database
        List<Point> points = Database.fetchPointsFromDatabase();

        // If there are no points, exit the program
        if (points.isEmpty()) {
            System.out.println("No points found in the database.");
            return;
        }

        // The start point of the robot
        Point start = points.remove(0);

        // Apply the Nearest Neighbor algorithm
        List<Point> path = NearestNeighbor.nearestNeighborAlgorithm(start, points);

        // Print the found path
        System.out.println("Gevonden pad:");
        for (Point p : path) {
            System.out.println("Punt: (" + p.x + ", " + p.y + ")");
        }
    }
}
