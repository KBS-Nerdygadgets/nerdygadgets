package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import Database.Database;
import TSP.NearestNeighbor;
import java.util.List;

public class ControlPanel extends JPanel{
    private Dimension screenDimension;
    private GridPoint robotLocation;
    private JPanel topPanel, topPanelGrid, bottomPanelBottom, bottomPanelTop;
    private JLabel schapLabel, robotLocationLabel, unvisitedProductLabel, robotPathLabel, fetchedProductLabel, legendLabel;
    private int text = 1;
    
    public ControlPanel(Dimension screenDimension){

                // Punten halen van de database
                List<TSP.Point> points = Database.fetchPointsFromDatabase();
        
                // Als er geen punten zijn, stoppen
                if (points.isEmpty()) {
                    System.out.println("No points found in the database.");
                    return;
                }
        
                // Het startpunt van de robot
                TSP.Point start = points.remove(0);
        
                // De Nearest Neighbour Algorithm toepassen
                List<TSP.Point> path = NearestNeighbor.nearestNeighborAlgorithm(start, points);

        //De kleuren die in dit Paneel gebruikt worden. Deze worden met de integratie van Themas aangepast
        Color darkGray = new Color(51, 51, 51);
        Color background = new Color(35, 35, 35);
        Color borderColor = new Color(58, 58, 58);
        Color gridForeground = new Color(78, 78, 78);
        Color foregroundColor = new Color(204, 204, 204);
        //De borders die gebruikt worden
        Border gridBorder = BorderFactory.createLineBorder(gridForeground);
        //Dit paneel is verdeeld in twee verschillende panelen
        Dimension topPanelDimension = new Dimension(screenDimension.width, screenDimension.height/4*3);
        Dimension bottomPanelDimension = new Dimension(screenDimension.width, screenDimension.height/4);
        int bottomPanelLabelSize = 20;

        //Past het control paneel aan
        this.screenDimension = screenDimension;
        setBackground(darkGray);
        setLayout(new BorderLayout());
        setPreferredSize(screenDimension);

        //Het bovenste Paneel wordt aangemaakt en aangepast
        topPanel = new JPanel(new BorderLayout());
        topPanel.setBackground(darkGray);
        topPanel.setBorder(new EmptyBorder(10, 10, 10, 10));
        topPanel.setPreferredSize(topPanelDimension);

        //Het label bovenaan het bovenste paneel wordt aangemaakt en aangepast
        schapLabel = new JLabel("Shelf");
        schapLabel.setForeground(foregroundColor);
        schapLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, 40));
        schapLabel.setHorizontalAlignment(SwingConstants.CENTER);

        //Het grid voor de x en y coördinaten wordt aangemaakt en aangepast
        topPanelGrid = new JPanel(new GridLayout(5, 5));
        topPanelGrid.setBackground(background);
        topPanelGrid.setPreferredSize(topPanelDimension);
        topPanelGrid.setBorder(gridBorder);

        //Het middelste paneel voor de legend label wordt aangemaakt
        bottomPanelTop = new JPanel();
        bottomPanelTop.setBackground(darkGray);
        bottomPanelTop.setPreferredSize(new Dimension(bottomPanelDimension.width, bottomPanelDimension.height/10));

        //Het onderste paneel voor de 4 verschillende onderdelen van de legende wordt aangemaakt en aangepast
        bottomPanelBottom = new JPanel(new GridLayout(2, 2));
        bottomPanelBottom.setBackground(darkGray);
        bottomPanelBottom.setBorder(new EmptyBorder(10, 10, 10, 10));
        bottomPanelBottom.setPreferredSize(new Dimension(bottomPanelDimension.width, bottomPanelDimension.height/10*5));

        //Het legend label wordt aangemaakt
        legendLabel = new JLabel("Legend");
        legendLabel.setForeground(foregroundColor);
        legendLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, bottomPanelLabelSize));
        legendLabel.setHorizontalAlignment(SwingConstants.CENTER);

        //De Labels voor het onderste paneel worden aangemaakt en aangepast
        robotLocationLabel = new JLabel("Robot Location");
        robotLocationLabel.setForeground(Color.CYAN);
        robotLocationLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, bottomPanelLabelSize));
        robotLocationLabel.setHorizontalAlignment(SwingConstants.CENTER);

        unvisitedProductLabel = new JLabel("Unvisited Product");
        unvisitedProductLabel.setForeground(Color.RED);
        unvisitedProductLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, bottomPanelLabelSize));
        unvisitedProductLabel.setHorizontalAlignment(SwingConstants.CENTER);

        robotPathLabel = new JLabel("Robot Path");
        robotPathLabel.setForeground(Color.GREEN);
        robotPathLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, bottomPanelLabelSize));
        robotPathLabel.setHorizontalAlignment(SwingConstants.CENTER);

        fetchedProductLabel = new JLabel("Fetched product");
        fetchedProductLabel.setForeground(new Color(113, 33, 234));
        fetchedProductLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, bottomPanelLabelSize));
        fetchedProductLabel.setHorizontalAlignment(SwingConstants.CENTER);

        //De attributen van het bovenste paneel worden toegevoegd
        topPanel.add(schapLabel, BorderLayout.NORTH);
        topPanel.add(topPanelGrid, BorderLayout.CENTER);
        //De for loops zorgen ervoor dat er 25 labels aangemaakt worden die elk op de goede plaats in het grid geplaatst worden
        for(int y = 1; y < 6; y++){
            for(int x = 1; x < 6; x++){
                JLabel tempLabel = new JLabel(y+":"+x);
                tempLabel.setForeground(gridForeground);
                tempLabel.setFont(new Font(schapLabel.getFont().getName(), Font.PLAIN, 40));
                tempLabel.setHorizontalAlignment(SwingConstants.CENTER);
                tempLabel.setBorder(gridBorder);
                //Deze labels worden allemaal in het grid geplaatst
                topPanelGrid.add(tempLabel);
                //Als de x en y coördinaten overeenkomen met de x en y coördinaten van de robot, dan wordt de kleur van de label aangepast en de volgorde neergezet
                for (TSP.Point p : path) {
                    if (x == p.getY() && y == p.getX()) {
                        tempLabel.setText("U.P.");
                        tempLabel.setForeground(Color.RED);
                    }
                }
            }
        }

        //De labels worden in het onderste paneel toegevoegd
        bottomPanelTop.add(legendLabel);
        bottomPanelBottom.add(robotLocationLabel);
        bottomPanelBottom.add(unvisitedProductLabel);
        bottomPanelBottom.add(robotPathLabel);
        bottomPanelBottom.add(fetchedProductLabel);

        //Het bovenste en onderste panelen worden aan het control paneel toegevoegd
        add(topPanel, BorderLayout.NORTH);
        add(bottomPanelTop, BorderLayout.CENTER);
        add(bottomPanelBottom, BorderLayout.SOUTH);
    }
}
