#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace std;

// MODULE 1: CAR MODULE

class Car
{
private:
    int carID;
    char regNo[15];
    char model[30];
    char brand[20];
    float ratePerDay;
    int isAvailable;
    char fuelType[10];

public:
    int getCarID()
    {
        return carID;
    }

    float getRate()
    {
        return ratePerDay;
    }

    int getAvailability()
    {
        return isAvailable;
    }

    void setAvailability(int value)
    {
        isAvailable = value;
    }

    void inputCar()
    {
        cout << "\nEnter Car ID: ";
        cin >> carID;
        cout << "Enter Registration Number: ";
        cin >> regNo;
        cout << "Enter Model: ";
        cin >> model;
        cout << "Enter Brand: ";
        cin >> brand;
        cout << "Enter Rate Per Day: ";
        cin >> ratePerDay;
        cout << "Enter Fuel Type: ";
        cin >> fuelType;
        isAvailable = 1;
    }

    void displayCar()
    {
        cout << "\nCar ID: " << carID;
        cout << "\nRegistration Number: " << regNo;
        cout << "\nModel: " << model;
        cout << "\nBrand: " << brand;
        cout << "\nRate Per Day: " << ratePerDay;
        cout << "\nFuel Type: " << fuelType;

        if (isAvailable == 1)
            cout << "\nStatus: Available\n";
        else
            cout << "\nStatus: Rented\n";
    }

    void addCar()
    {
        inputCar();

        ofstream file("cars.dat", ios::binary | ios::app);

        if (!file)
        {
            cout << "\nError opening cars.dat!\n";
            return;
        }

        // Check duplicate Car ID
        file.close();

        ifstream check("cars.dat", ios::binary);
        Car temp;

        while (check.read((char*)&temp, sizeof(Car)))
        {
            if (temp.carID == carID)
            {
                check.close();
                cout << "\nCar ID already exists!\n";
                return;
            }
        }
        check.close();

        ofstream out("cars.dat", ios::binary | ios::app);
        out.write((char*)this, sizeof(Car));
        out.close();

        cout << "\nCar added successfully!\n";
    }

    void displayAllCars()
    {
        ifstream file("cars.dat", ios::binary);

        if (!file)
        {
            cout << "\nNo car records found!\n";
            return;
        }

        Car temp;

        while (file.read((char*)&temp, sizeof(Car)))
        {
            temp.displayCar();
        }

        file.close();
    }

    void searchCar()
    {
        int id;
        bool found = false;

        cout << "\nEnter Car ID to Search: ";
        cin >> id;

        ifstream file("cars.dat", ios::binary);
        Car temp;

        if (!file)
        {
            cout << "\nFile not found!\n";
            return;
        }

        file.seekg(0, ios::beg);

        while (file.read((char*)&temp, sizeof(Car)))
        {
            if (temp.carID == id)
            {
                temp.displayCar();
                found = true;
                break;
            }
        }

        file.close();

        if (!found)
            cout << "\nCar not found!\n";
    }

    void updateCar()
    {
        int id;
        bool found = false;

        cout << "\nEnter Car ID to Update: ";
        cin >> id;

        fstream file("cars.dat", ios::binary | ios::in | ios::out);

        if (!file)
        {
            cout << "\nFile not found!\n";
            return;
        }

        Car temp;

        while (file.read((char*)&temp, sizeof(Car)))
        {
            if (temp.carID == id)
            {
                found = true;
                temp.displayCar();

                cout << "\nEnter New Registration Number: ";
                cin >> temp.regNo;
                cout << "Enter New Model: ";
                cin >> temp.model;
                cout << "Enter New Brand: ";
                cin >> temp.brand;
                cout << "Enter New Rate Per Day: ";
                cin >> temp.ratePerDay;
                cout << "Enter New Fuel Type: ";
                cin >> temp.fuelType;

                streampos position = file.tellg();
                position -= static_cast<streamoff>(sizeof(Car));

                file.clear();
                file.seekp(position);
                file.write((char*)&temp, sizeof(Car));

                cout << "\nCar updated successfully!\n";
                break;
            }
        }

        file.close();

        if (!found)
            cout << "\nCar not found!\n";
    }

    void deleteCar()
    {
        int id;
        bool found = false;

        cout << "\nEnter Car ID to Delete: ";
        cin >> id;

        ifstream file("cars.dat", ios::binary);

        if (!file)
        {
            cout << "\nFile not found!\n";
            return;
        }

        ofstream tempFile("temp.dat", ios::binary);

        if (!tempFile)
        {
            cout << "\nTemporary file could not be created!\n";
            file.close();
            return;
        }

        Car temp;

        while (file.read((char*)&temp, sizeof(Car)))
        {
            if (temp.carID == id)
                found = true;
            else
                tempFile.write((char*)&temp, sizeof(Car));
        }

        file.close();
        tempFile.close();

        if (found)
        {
            remove("cars.dat");
            rename("temp.dat", "cars.dat");
            cout << "\nCar deleted successfully!\n";
        }
        else
        {
            remove("temp.dat");
            cout << "\nCar not found!\n";
        }
    }

    void updateAvailabilityInFile(int id, int value)
    {
        fstream file("cars.dat", ios::binary | ios::in | ios::out);

        if (!file)
        {
            cout << "\nUnable to update car file!\n";
            return;
        }

        Car temp;

        while (file.read((char*)&temp, sizeof(Car)))
        {
            if (temp.carID == id)
            {
                temp.setAvailability(value);

                streampos position = file.tellg();
                position -= static_cast<streamoff>(sizeof(Car));

                file.clear();
                file.seekp(position);
                file.write((char*)&temp, sizeof(Car));
                break;
            }
        }

        file.close();
    }

    bool getCarForRental(int id, float &rate)
    {
        ifstream file("cars.dat", ios::binary);

        if (!file)
            return false;

        Car temp;

        while (file.read((char*)&temp, sizeof(Car)))
        {
            if (temp.carID == id && temp.isAvailable == 1)
            {
                rate = temp.ratePerDay;
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }
};

// MODULE 2: RENTAL MODULE

class Rental
{
private:
    int rentalID;
    int carID;
    char customerName[50];
    char contactNo[15];
    char startDate[15];
    char endDate[25];
    int noOfDays;
    float totalCharge;
    char status[12];

public:
    int getCarID()
    {
    return carID;
    }

    void displayRental()
    {
        cout << "\nRental ID: " << rentalID;
        cout << "\nCar ID: " << carID;
        cout << "\nCustomer Name: " << customerName;
        cout << "\nContact Number: " << contactNo;
        cout << "\nStart Date: " << startDate;
        cout << "\nEnd Date: " << endDate;
        cout << "\nNumber of Days: " << noOfDays;
        cout << "\nTotal Charge: " << totalCharge;
        cout << "\nStatus: " << status << "\n";
    }

    bool rentalIDExists(int id)
    {
        ifstream file("rentals.dat", ios::binary);

        if (!file)
            return false;

        Rental temp;

        while (file.read((char*)&temp, sizeof(Rental)))
        {
            if (temp.rentalID == id)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    void createRental(Car &car)
    {
        float rate;

        cout << "\nEnter Rental ID: ";
        cin >> rentalID;

        if (rentalIDExists(rentalID))
        {
            cout << "\nRental ID already exists!\n";
            return;
        }

        cout << "Enter Car ID: ";
        cin >> carID;

        if (!car.getCarForRental(carID, rate))
        {
            cout << "\nCar not found or already rented!\n";
            return;
        }

        cout << "Enter Customer Name (one word): ";
        cin >> customerName;
        cout << "Enter Contact Number: ";
        cin >> contactNo;
        cout << "Enter Start Date (DD-MM-YYYY): ";
        cin >> startDate;
        cout << "Enter End Date (DD-MM-YYYY): ";
        cin >> endDate;
        cout << "Enter Number of Days: ";
        cin >> noOfDays;

        if (noOfDays <= 0)
        {
            cout << "\nNumber of days must be greater than zero!\n";
            return;
        }

        totalCharge = rate * noOfDays;
        strcpy(status, "ACTIVE");

        ofstream file("rentals.dat", ios::binary | ios::app);

        if (!file)
        {
            cout << "\nError opening rentals.dat!\n";
            return;
        }

        // tellp() gives the current write position
        streampos position = file.tellp();

        file.write((char*)this, sizeof(Rental));
        file.close();

        if (!file)
        {
            cout << "\nRental could not be saved!\n";
            return;
        }

        car.updateAvailabilityInFile(carID, 0);

        cout << "\nRental created successfully!\n";
        cout << "Record Position: " << position << endl;
        cout << "Total Charge: " << totalCharge << endl;
    }

    void displayAllRentals()
    {
        ifstream file("rentals.dat", ios::binary);

        if (!file)
        {
            cout << "\nNo rental records found!\n";
            return;
        }

        Rental temp;

        while (file.read((char*)&temp, sizeof(Rental)))
        {
            temp.displayRental();
        }

        file.close();
    }

    void searchRental()
    {
        int id;
        bool found = false;

        cout << "\nEnter Rental ID to Search: ";
        cin >> id;

        ifstream file("rentals.dat", ios::binary);

        if (!file)
        {
            cout << "\nRental file not found!\n";
            return;
        }

        Rental temp;
        file.seekg(0, ios::beg);

        while (file.read((char*)&temp, sizeof(Rental)))
        {
            if (temp.rentalID == id)
            {
                cout << "\nRental Found!\n";
                temp.displayRental();
                found = true;
                break;
            }
        }

        file.close();

        if (!found)
            cout << "\nRental not found!\n";
    }

    void returnCar(Car &car)
    {
        int id;
        bool found = false;

        cout << "\nEnter Rental ID to Return: ";
        cin >> id;

        fstream file("rentals.dat", ios::binary | ios::in | ios::out);

        if (!file)
        {
            cout << "\nRental file not found!\n";
            return;
        }

        Rental temp;

        while (file.read((char*)&temp, sizeof(Rental)))
        {
            if (temp.rentalID == id)
            {
                found = true;

                if (strcmp(temp.status, "RETURNED") == 0)
                {
                    cout << "\nThis rental has already been returned!\n";
                    file.close();
                    return;
                }

                temp.displayRental();
                strcpy(temp.status, "RETURNED");

                // tellg() gives the position after reading the record
                streampos position = file.tellg();
                position -= static_cast<streamoff>(sizeof(Rental));

                file.clear();
                file.seekp(position);
                file.write((char*)&temp, sizeof(Rental));
                file.close();

                car.updateAvailabilityInFile(temp.carID, 1);

                cout << "\nCar returned successfully!\n";
                return;
            }
        }

        file.close();

        if (!found)
            cout << "\nRental not found!\n";
    }
};

 // MODULE 3: REPORT MODULE

class Report
{
public:

    // 1. Display Available Cars
    void displayAvailableCars()
    {
        ifstream file("cars.dat", ios::binary);

        if (!file)
        {
            cout << "\nNo car records found!\n";
            return;
        }

        Car temp;
        bool found = false;

        cout << "\n===== AVAILABLE CARS REPORT =====\n";

        while (file.read((char*)&temp, sizeof(Car)))
        {
            if (temp.getAvailability() == 1)
            {
                temp.displayCar();
                found = true;
            }
        }

        file.close();

        if (!found)
            cout << "\nNo available cars!\n";
    }

    // 2. Display Rental History
    void displayRentalHistory()
    {
        ifstream file("rentals.dat", ios::binary);

        if (!file)
        {
            cout << "\nNo rental records found!\n";
            return;
        }

        Rental temp;
        bool found = false;

        cout << "\n===== RENTAL HISTORY REPORT =====\n";

        while (file.read((char*)&temp, sizeof(Rental)))
        {
            temp.displayRental();
            found = true;
        }

        file.close();

        if (!found)
            cout << "\nNo rental history found!\n";
    }

    // 3. Search Rental History by Car ID
    void searchHistoryByCarID()
    {
        int id;
        bool found = false;

        cout << "\nEnter Car ID: ";
        cin >> id;

        ifstream file("rentals.dat", ios::binary);

        if (!file)
        {
            cout << "\nNo rental records found!\n";
            return;
        }

        Rental temp;

        cout << "\n===== CAR RENTAL HISTORY =====\n";

        while (file.read((char*)&temp, sizeof(Rental)))
        {
            // Rental class ke private Car ID ko getter se access karenge
            if (temp.getCarID() == id)
            {
                temp.displayRental();
                found = true;
            }
        }

        file.close();

        if (!found)
            cout << "\nNo rental history for this Car ID!\n";
    }
};


// MAIN MENU

int main()
{
    Car car;
    Rental rental;
    Report report;
    int mainChoice, choice;

    do
    {
        cout << "\n=============================\n";
        cout << "  CAR RENTAL MANAGEMENT SYSTEM\n";
        cout << "=============================\n";
        cout << "1. Car Module\n";
        cout << "2. Rental Module\n";
        cout << "3. Report Module\n";
        cout << "4. Exit\n";
        cout << "Enter Choice: ";
        cin >> mainChoice;

        switch (mainChoice)
        {
            case 1:
                do
                {
                    cout << "\n===== CAR MODULE MENU =====\n";
                    cout << "1. Add Car\n";
                    cout << "2. Display All Cars\n";
                    cout << "3. Search Car\n";
                    cout << "4. Update Car\n";
                    cout << "5. Delete Car\n";
                    cout << "6. Back to Main Menu\n";
                    cout << "Enter Choice: ";
                    cin >> choice;

                    switch (choice)
                    {
                        case 1: car.addCar(); break;
                        case 2: car.displayAllCars(); break;
                        case 3: car.searchCar(); break;
                        case 4: car.updateCar(); break;
                        case 5: car.deleteCar(); break;
                        case 6: cout << "\nReturning to Main Menu.\n"; break;
                        default: cout << "\nInvalid Choice!\n";
                    }

                } while (choice != 6);
                break;

            case 2:
                do
                {
                    cout << "\n===== RENTAL MODULE MENU =====\n";
                    cout << "1. Create Rental\n";
                    cout << "2. Display All Rentals\n";
                    cout << "3. Search Rental\n";
                    cout << "4. Return Car\n";
                    cout << "5. Back to Main Menu\n";
                    cout << "Enter Choice: ";
                    cin >> choice;

                    switch (choice)
                    {
                        case 1: rental.createRental(car); break;
                        case 2: rental.displayAllRentals(); break;
                        case 3: rental.searchRental(); break;
                        case 4: rental.returnCar(car); break;
                        case 5: cout << "\nReturning to Main Menu.\n"; break;
                        default: cout << "\nInvalid Choice!\n";
                    }

                } while (choice != 5);
                break;

            case 3:
            do
            {
                cout << "\n===== REPORT MODULE MENU =====\n";
                cout << "1. Display Available Cars\n";
                cout << "2. Display Rental History\n";
                cout << "3. Search Rental History by Car ID\n";
                cout << "4. Back to Main Menu\n";
                cout << "Enter Choice: ";
                cin >> choice;

                switch (choice)
               {
                   case 1:
                    report.displayAvailableCars();
                    break;

                  case 2:
                    report.displayRentalHistory();
                    break;

                 case 3:
                    report.searchHistoryByCarID();
                    break;

                 case 4:
                    cout << "\nReturning to Main Menu.\n";
                    break;

                 default:
                    cout << "\nInvalid Choice!\n";
                }

            } while (choice != 4);
                break;
                
            case 4:
                cout << "\nCar Rental Management System Closed.\n";
                break;
        }

    } while (mainChoice != 4);

    return 0;
}