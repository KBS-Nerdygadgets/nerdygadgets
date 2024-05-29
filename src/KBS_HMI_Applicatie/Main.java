package KBS_HMI_Applicatie;

import com.fazecast.jSerialComm.SerialPort;

import java.awt.*;
import java.util.Scanner;


public class Main {
    public static void main(String[] args) {
        new GUIManager(new Dimension(1000, 800));

        SerialComm();
    }

    public static void SerialComm() {
        //Maak lijst met poorten
        SerialPort[] ports = SerialPort.getCommPorts();

        if (ports.length == 0) {
            System.out.println("Geen poorten gevonden!");
            return;
        }

        //Laat poorten zien en laat gebruiker kiezen
        System.out.println("Beschikbare poorten:");
        for (int i = 0; i < ports.length; i++) {
            System.out.println((i + 1) + ". " + ports[i].getDescriptivePortName());
        }

        Scanner scanner = new Scanner(System.in);
        int choice = -1;

        do {
            System.out.print("Geef het nummer van de poort die je wilt gebruiken (1-" + ports.length + "): ");
            try {
                choice = Integer.parseInt(scanner.nextLine().trim());
            } catch (NumberFormatException e) {
                System.out.println("Geef een geldig nummer.");
            }

            if (choice < 1 || choice > ports.length) {
                System.out.println("Geef een nummer tussen 1 en " + ports.length + ".");
            }
        } while (choice < 1 || choice > ports.length);

        //Selecteer gedefineerde poort
        SerialPort port = ports[choice - 1];

        //Open poort
        if (port.openPort()) {
            System.out.println("Serial port succesvol geopend.");
        } else {
            System.out.println("Error, kon poort niet openen.");
            scanner.close();
            return;
        }

        //Zet baudrate op de baudrate waar arduino ontvangt
        port.setBaudRate(9600);

        try {
            while (true) {
                System.out.print("Geef locatie (bijv. 1:2): ");
                String input = scanner.nextLine().trim();
                port.getOutputStream().write(input.getBytes());
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            //Sluit poorten
            port.closePort();
            scanner.close();
        }
    }

}
