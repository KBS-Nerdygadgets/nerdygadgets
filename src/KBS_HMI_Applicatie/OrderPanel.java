package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.util.ArrayList;
import Database.Database;
import Bin_Packing.*;
import TSP.Point;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class OrderPanel extends JPanel{
    private JPanel ordersPanel, buttonsPanel, managebuttonPanel, startbuttonPanel;
    private JComboBox<String> selectOrderBox;
    private JButton startOrderButton;//, selectOrderBox;
    private JLabel orderLabel, orders;
    private JComboBox<String> selectedPointsBox;
    private JButton confirmSelectionButton;


    public OrderPanel(Dimension screenDimension){
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

// De knop die de start order knop in het startOrderButton
        startOrderButton = new JButton("Start Order");
        startOrderButton.setBackground(buttonColor);
        startOrderButton.setForeground(foregroundColor);
        startOrderButton.setPreferredSize(buttonDimensions);
        startOrderButton.setFont(new Font(startOrderButton.getFont().getName(), Font.PLAIN, 20));
        startOrderButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Verplaats de geselecteerde items naar orderLabel
                StringBuilder orderText = new StringBuilder("Order: ");
                for (int i = 0; i < selectedPointsBox.getItemCount(); i++) {
                    int OrderID = Integer.valueOf(selectedPointsBox.getItemAt(i));
                    orderText.append(selectedPointsBox.getItemAt(i));
                    //System.out.println(Database.fetchStockItemIDFromDatabase(Integer.valueOf(selectedPointsBox.getItemAt(i))));
                    //binpacking
                    ArrayList<ArrayList<Integer>> BPParray = First_Fit_Decreasing.firstFitDecreasing(Database.fetchStockItemIDFromDatabase(Integer.valueOf(selectedPointsBox.getItemAt(i))));
                    System.out.println(BPParray);
                    ArrayList<ArrayList<Integer>> BPPenTSParray = TSP.Main.TSPfuncties(BPParray, OrderID);
                    System.out.println(BPPenTSParray);
                    String JavaToArduino = "";
                    for(int rit = 0; rit < BPPenTSParray.size(); rit++){
                        for(int locatie = 0; locatie < BPPenTSParray.get(rit).size(); locatie++){
                            JavaToArduino += BPPenTSParray.get(rit).get(locatie) + ",";
                        }
                        JavaToArduino += "|";
                    }
                    System.out.println(JavaToArduino);
                    GUIManager.scmPanel.sendToSelectedPort(JavaToArduino);
                    System.out.println("");
                    if (i < selectedPointsBox.getItemCount() - 1) {
                        orderText.append(", ");
                    }
                }
                orderLabel.setText(orderText.toString());
                // Wis de selectie in selectedPointsBox
                selectedPointsBox.removeAllItems();
                // Reset de bevestigingsknop zodat deze weer kan worden gebruikt
                confirmSelectionButton.setEnabled(true);
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

        // De combobox voor de geselecteerde punten
        selectedPointsBox = new JComboBox<>();
        selectedPointsBox.setBackground(buttonColor);
        selectedPointsBox.setForeground(foregroundColor);
        selectedPointsBox.setPreferredSize(buttonDimensions);
        selectedPointsBox.setFont(new Font(selectedPointsBox.getFont().getName(), Font.PLAIN, 20));

// De knop om de selectie te bevestigen
        confirmSelectionButton = new JButton("Confirm");
        confirmSelectionButton.setBackground(buttonColor);
        confirmSelectionButton.setForeground(foregroundColor);
        confirmSelectionButton.setPreferredSize(buttonDimensions);
        confirmSelectionButton.setFont(new Font(confirmSelectionButton.getFont().getName(), Font.PLAIN, 20));
        confirmSelectionButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Haal de geselecteerde waarde van de selectOrderBox op
                Object selectedObject = selectOrderBox.getSelectedItem();
                if (selectedObject != null) {
                    String selectedOrder = (String) selectedObject;
                    // Voeg de geselecteerde waarde toe aan de selectedPointsBox als deze nog niet is toegevoegd en er minder dan 3 items zijn
                    if (selectedPointsBox.getItemCount() < 3) {
                        // Controleer of het geselecteerde item niet al in selectedPointsBox zit
                        boolean alreadyAdded = false;
                        for (int i = 0; i < selectedPointsBox.getItemCount(); i++) {
                            if (selectedOrder.equals(selectedPointsBox.getItemAt(i))) {
                                alreadyAdded = true;
                                break;
                            }
                        }
                        if (!alreadyAdded) {
                            selectedPointsBox.addItem(selectedOrder);
                        }
                        // Als er 3 items zijn toegevoegd, blokkeer verdere toevoegingen
                        if (selectedPointsBox.getItemCount() == 3) {
                            confirmSelectionButton.setEnabled(false);
                        }
                    }
                }
            }
        });

        //Alles wordt toegevoegd in de panelen
        add(orders, BorderLayout.NORTH);
        add(ordersPanel, BorderLayout.CENTER);
        ordersPanel.add(orderLabel, BorderLayout.NORTH);
        ordersPanel.add(buttonsPanel, BorderLayout.CENTER);

        buttonsPanel.add(managebuttonPanel, BorderLayout.WEST);
        managebuttonPanel.add(selectOrderBox);
        managebuttonPanel.add(confirmSelectionButton);

        buttonsPanel.add(startbuttonPanel, BorderLayout.EAST);
        startbuttonPanel.add(selectedPointsBox);
        startbuttonPanel.add(startOrderButton);
    }

    private void populateComboBox() {
        int[] orderIDs = Database.fetchOrderIDsFromDatabase();
        for (int orderID : orderIDs) {
            selectOrderBox.addItem(Integer.toString(orderID));
        }
    }
}