package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.util.List;
import TSP.Main;
import TSP.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class OrderPanel extends JPanel{
    private GUIThemes guiTheme;
    private Dimension screenDimension;
    private JPanel ordersPanel, buttonsPanel, managebuttonPanel, startbuttonPanel;
    private JComboBox<String> selectOrderBox;
    private JButton startOrderButton;//, selectOrderBox;
    private JLabel orderLabel, orders;

    public OrderPanel(Dimension screenDimension){
        this.screenDimension = screenDimension;
        //De kleuren die in dit Paneel gebruikt worden. Deze worden met de integratie van Themas aangepast
        Color darkGray = new Color(51, 51, 51);
        Color background = new Color(35, 35, 35);
        Color borderColor = new Color(58, 58, 58);
        Color foregroundColor = new Color(204, 204, 204);
        Color buttonColor = new Color(73, 73, 73);
        //De borders en dimensions die gebruikt worden
        Border border = BorderFactory.createLineBorder(borderColor);
        EmptyBorder panelBorderDimensions = new EmptyBorder(10, 10, 5, 10);
        Dimension buttonDimensions = new Dimension(180, 40);
        Dimension buttonPanelDimensions = new Dimension(screenDimension.width/2-21, screenDimension.height);

        //Past het order paneel aan
        setBackground(darkGray);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(0, 10, 10, 10));
        setPreferredSize(screenDimension);

        //De label bovenaan de order paneel wordt aangemaakt
        orders = new JLabel("Orders");
        orders.setForeground(foregroundColor);
        orders.setFont(new Font(orders.getFont().getName(), Font.PLAIN, 30));
        orders.setHorizontalAlignment(SwingConstants.CENTER);

        //Het paneel onderaan de orderpaneel wordt aangemaakt
        ordersPanel = new JPanel(new BorderLayout());
        ordersPanel.setBackground(background);
        ordersPanel.setBorder(border);

        //Het label bovenaan het onderste paneel wordt aangemaakt
        orderLabel = new JLabel("Order: none");
        orderLabel.setForeground(foregroundColor);
        orderLabel.setFont(new Font(orders.getFont().getName(), Font.PLAIN, 20));
        orderLabel.setHorizontalAlignment(SwingConstants.CENTER);

        //Er wordt nog een paneel onderaan het onderste paneel aangemaakt. Deze zal de knoppen bevatten
        buttonsPanel = new JPanel(new BorderLayout());
        buttonsPanel.setBackground(background);
        buttonsPanel.setBorder(panelBorderDimensions);
        buttonsPanel.setPreferredSize(new Dimension(screenDimension.width, 3));

        //Een paneel voor de manage knop
        managebuttonPanel = new JPanel(new FlowLayout());
        managebuttonPanel.setBackground(background);
        managebuttonPanel.setBorder(panelBorderDimensions);
        managebuttonPanel.setPreferredSize(buttonPanelDimensions);

        //Een paneel voor de start knop
        startbuttonPanel = new JPanel(new FlowLayout());
        startbuttonPanel.setBackground(background);
        startbuttonPanel.setBorder(panelBorderDimensions);
        startbuttonPanel.setPreferredSize(buttonPanelDimensions);

        //De knop die in het manage paneel geplaatst zal worden
        startOrderButton = new JButton("Start Order");
        startOrderButton.setBackground(buttonColor);
        startOrderButton.setForeground(foregroundColor);
        startOrderButton.setPreferredSize(buttonDimensions);
        startOrderButton.setFont(new Font(startOrderButton.getFont().getName(), Font.PLAIN, 20));
        startOrderButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Haal de geselecteerde waarde van de JComboBox op
                String selectedOrder = (String) selectOrderBox.getSelectedItem();
                // Werk de tekst van orderLabel bij met de geselecteerde waarde
                orderLabel.setText("Order: " + selectedOrder);
            }
        });

        // De combobox die in het start paneel geplaatst zal worden
        selectOrderBox = new JComboBox<>();
        selectOrderBox.setBackground(buttonColor);
        selectOrderBox.setForeground(foregroundColor);
        selectOrderBox.setPreferredSize(buttonDimensions);
        selectOrderBox.setFont(new Font(selectOrderBox.getFont().getName(), Font.PLAIN, 20));
        // Fetch and populate points in the combobox
        populateComboBox();

        //De knop die in het start paneel geplaatst zal worden
//        selectOrderBox = new JButton("Start order");
//        selectOrderBox.setBackground(buttonColor);
//        selectOrderBox.setForeground(foregroundColor);
//        selectOrderBox.setPreferredSize(buttonDimensions);
//        selectOrderBox.setFont(new Font(selectOrderBox.getFont().getName(), Font.PLAIN, 20));

        //Alles wordt toegevoegd in de panelen
        add(orders, BorderLayout.NORTH);
        add(ordersPanel, BorderLayout.CENTER);
        ordersPanel.add(orderLabel, BorderLayout.NORTH);
        ordersPanel.add(buttonsPanel, BorderLayout.CENTER);

        buttonsPanel.add(managebuttonPanel, BorderLayout.WEST);
        startbuttonPanel.add(startOrderButton);

        buttonsPanel.add(startbuttonPanel, BorderLayout.EAST);
        managebuttonPanel.add(selectOrderBox);
    }

    private void populateComboBox() {
        // Fetch points using the TSP.Main class
        List<Point> points = Main.fetchPoints();
        // Remove the first point (start point) if needed
        if (!points.isEmpty()) {
            points.remove(0);
        }
        // Convert points to string and add to combobox
        for (Point p : points) {
            selectOrderBox.addItem("(" + p.x + ", " + p.y + ")");
        }
    }
}
