#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>


#define MAX_CREW 100
#define MAIN_PASS "NVAPROJECTBYZIHADI"
#define RESTRICTED_PASS "NVAMOREACCESS"
#define ADMIN_PASS "NVAADMINACCESS"

typedef struct {
    int id;
    char name[50];
    int age;
    char role[50];
    char training[50];
} Crew;

void color(int code) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), code);
}

void welcomeUser() {
    printf("\a");
    color(11);
    printf(" Welcome to the Space Mission Resource Management System! \n");
    printf("--------------------------------------------------------\n");
    color(15);
}

int captcha() {
    int a = rand() % 10;
    int b = rand() % 10;
    int answer;
    printf("Captcha: What is %d + %d?\n", a, b);
    scanf("%d", &answer);
    return answer == (a + b);
}

int authenticate() {
    char name[50], passkey[50];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your access passkey: ");
    scanf("%s", passkey);
    if (strcmp(passkey, MAIN_PASS) != 0) {
        color(12);
        printf(" Access Denied.\n");
        color(15);
        if (captcha()) {
            color(14);
            printf(" Captcha passed. Limited access granted.\n");
            color(15);
            return 1;
        } else {
            color(12);
            printf(" Captcha failed. Access denied permanently.\n");
            color(15);
            exit(0);
        }
    }
    color(10);
    printf(" Access Granted. Welcome, %s!\n", name);
    color(15);
    return 1;
}

int authenticateAdmin() {
    char adminPass[50];
    printf("Enter admin access passkey: ");
    scanf("%s", adminPass);
    if (strcmp(adminPass, ADMIN_PASS) == 0) {
        color(10);
        printf(" Admin access granted.\n");
        color(15);
        return 1;
    } else {
        color(12);
        printf(" Incorrect admin password. Access denied.\n");
        color(15);
        return 0;
    }
}



void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void registerCrewMember() {
    FILE *file = fopen("crew.txt", "a");
    if (!file) {
        printf("Could not open crew file.\n");
        return;
    }

    int numMembers, id, age;
    char name[100], role[100], experience[100];

    color(10);
    printf("\nHow many crew members do you want to register? ");
    color(15);
    scanf("%d", &numMembers);
    getchar();

    for (int i = 0; i < numMembers; i++) {
        color(10);
        printf("\nEnter Crew ID for member %d: ", i + 1);
        color(15);
        scanf("%d", &id);
        getchar();

        color(10);
        printf("Enter Name for member %d: ", i + 1);
        color(15);
        fgets(name, sizeof(name), stdin);
        trimNewline(name);

        color(10);
        printf("Enter Age for member %d: ", i + 1);
        color(15);
        scanf("%d", &age);
        getchar();

        color(10);
        printf("Enter Role for member %d: ", i + 1);
        color(15);
        fgets(role, sizeof(role), stdin);
        trimNewline(role);

        color(10);
        printf("Enter Experience Level for member %d: ", i + 1);
        color(15);
        fgets(experience, sizeof(experience), stdin);
        trimNewline(experience);

        fprintf(file, "%d|%s|%d|%s|%s\n", id, name, age, role, experience);

        color(10);
        printf("Crew member %d registered successfully!\n", i + 1);
        color(15);
    }

    fclose(file);
}

void viewCrewLogs() {
    FILE *file = fopen("crew.txt", "r");
    if (!file) {
        printf("Could not open crew file.\n");
        return;
    }

    int id, age;
    char line[512];
    char name[100], role[100], experience[100];

    color(9);
    printf("\n%-10s %-20s %-10s %-20s %-20s\n", "ID", "Name", "Age", "Role", "Experience Level");
    printf("-------------------------------------------------------------------------------\n");
    color(15);

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d|%99[^|]|%d|%99[^|]|%99[^\n]", &id, name, &age, role, experience) == 5) {
            printf("%-10d %-20s %-10d %-20s %-20s\n", id, name, age, role, experience);
        } else {
            printf("Invalid entry found in file, skipping.\n");
        }
    }

    fclose(file);
}



void checkResources() {
    float food, oxygen, fuel, water;
    printf("Enter current levels (percentage):\n");
    printf("Food: ");
    scanf("%f", &food);
    printf("Oxygen: ");
    scanf("%f", &oxygen);
    printf("Fuel: ");
    scanf("%f", &fuel);
    printf("Water: ");
    scanf("%f", &water);
    getchar();

    printf("\nResource Status:\n");
    if (food < 30) printf("  Food is low!\n");
    if (oxygen < 30) printf("  Oxygen is low!\n");
    if (fuel < 30) printf(" Fuel is low!\n");
    if (water < 30) printf("  Water is low!\n");
    if (food >= 30 && oxygen >= 30 && fuel >= 30 && water >= 30)
        printf(" All resources are in good condition.\n");
}

void viewPersonalInfo() {
    char pass[50];
    printf("Enter restricted access passkey: ");
    scanf("%s", pass);
    if (strcmp(pass, RESTRICTED_PASS) == 0) {
        color(10);
        printf(" Access granted to personal information.\n");
        printf("[Displaying restricted crew info...]\n");
        color(15);
    } else {
        color(12);
        printf(" Incorrect password. Access denied.\n");
        color(15);
    }
}

void viewMissionDetails() {
    color(13);
    printf(" Mission: NVA-01\nObjective: Explore the asteroid belt.\n");
    color(15);
}





void deleteCrewMember() {
    int idToDelete;
    printf("Enter the ID of the crew member to delete: ");
    scanf("%d", &idToDelete);
    FILE *file = fopen("crew.txt", "r");
    if (!file) {
        color(12);
        printf(" Error: Could not open crew file.\n");
        color(15);
        return;
    }
    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        color(12);
        printf(" Error: Could not create temporary file.\n");
        color(15);
        fclose(file);
        return;
    }
    char line[150];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        int id, age;
        char name[50], role[50], training[50];
        sscanf(line, "%d,%49[^,],%d,%49[^,],%49[^\n]", &id, name, &age, role, training);
        if (id != idToDelete) {
            fprintf(tempFile, "%d,%s,%d,%s,%s\n", id, name, age, role, training);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove("crew.txt");
        rename("temp.txt", "crew.txt");
        color(10);
        printf(" Crew member with ID %d has been successfully deleted.\n", idToDelete);
        color(15);
    } else {
        remove("temp.txt");
        color(12);
        printf(" Crew member with ID %d not found.\n", idToDelete);
        color(15);
    }
}

void checkSupplies() {
    int oxygen, food, fuel;
    printf("Enter oxygen level (0-100): ");
    scanf("%d", &oxygen);
    printf("Enter food provisions level (0-100): ");
    scanf("%d", &food);
    printf("Enter fuel level (0-100): ");
    scanf("%d", &fuel);
    if (oxygen < 30 || food < 40 || fuel < 35) {
        color(12);
        printf("⚠️ Warning: Supply levels are critically low!\n");
        if (oxygen < 30) printf("Oxygen below threshold.\n");
        if (food < 40) printf("Food below threshold.\n");
        if (fuel < 35) printf("Fuel below threshold.\n");
        color(15);
    } else {
        color(10);
        printf(" All supply levels are within safe thresholds.\n");
        color(15);
    }
}

void faultDetection() {
    float air_quality, equipment_integrity;
    printf("Enter chemical air composition rating (0.0 - 1.0): ");
    scanf("%f", &air_quality);
    printf("Enter equipment integrity rating (0.0 - 1.0): ");
    scanf("%f", &equipment_integrity);
    if (air_quality < 0.5 || equipment_integrity < 0.6) {
        color(12);
        printf(" Warning: Possible fault detected in the spaceship.\n");
        if (air_quality < 0.5) printf("Air composition not safe.\n");
        if (equipment_integrity < 0.6) printf("Equipment inconsistency detected.\n");
        color(15);
    } else {
        color(10);
        printf("Spaceship environment stable.\n");
        color(15);
    }
}

void calculateTrajectory() {
    float x, y, z;
    printf("Enter current coordinates (x y z): ");
    scanf("%f %f %f", &x, &y, &z);
    float newX = x + 5.0;
    float newY = y + 3.0;
    float newZ = z + 2.0;
    printf("New suggested trajectory coordinates: (%.2f, %.2f, %.2f)\n", newX, newY, newZ);
}

void addDetails() {
    FILE *fp = fopen("details.txt", "a");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    int count;
    char paragraph[1000];
    time_t now;
    struct tm *time_info;
    char time_string[9];

    time(&now);
    time_info = localtime(&now);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
    printf("Mission Time: %s\n", time_string);

    printf("How many paragraphs to add? ");
    scanf("%d", &count);
    getchar();

    for (int i = 0; i < count; i++) {
        printf("Enter paragraph %d: ", i + 1);
        fgets(paragraph, sizeof(paragraph), stdin);
        fprintf(fp, "%s", paragraph);
    }

    fclose(fp);
    printf("Mission details recorded.\n");
}
void viewDetails() {
    FILE *fp = fopen("details.txt", "r");
    if (fp == NULL) {
        printf("No mission details found.\n");
        return;
    }

    char line[1024];
    color(9);
    printf("\nMission Details:\n");
    color(15);
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}
void searchCrewMemberByID(int searchID) {
    FILE *file = fopen("crew.txt", "r");
    if (!file) {
        printf("Could not open crew file.\n");
        return;
    }

    int id, age;
    char line[512];
    char name[100], role[100], experience[100];
    int found = 0;

    color(9);
    printf("\n%-10s %-20s %-10s %-20s %-20s\n", "ID", "Name", "Age", "Role", "Experience Level");
    printf("-------------------------------------------------------------------------------\n");
    color(15);

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d|%99[^|]|%d|%99[^|]|%99[^\n]", &id, name, &age, role, experience) == 5) {
            if (id == searchID) {
                printf("%-10d %-20s %-10d %-20s %-20s\n", id, name, age, role, experience);
                found = 1;
                break;
            }
        } else {
            printf("Invalid entry found in file, skipping.\n");
        }
    }

    if (!found) {
        printf("No crew member found with ID: %d\n", searchID);
    }

    fclose(file);
}



int main() {
    srand(time(NULL));
    int choice, adminChoice;
    welcomeUser();
    authenticate();
    int id;

    do {
        printf("\nSelect an option:\n");
        printf("1. View Crew Logs\n");
        printf("2. Check Resources\n");
        printf("3. View Personal Information\n");
        printf("4. View Mission Details\n");
        printf("5. Register New Crew Member\n");
        printf("6. Delete Crew Member\n");
        printf("7. Check Supplies\n");
        printf("8. Fault Detection\n");
        printf("9. Calculate Trajectory\n");
        printf("10. Add Details\n");

        printf("11. Admin Login\n");
        printf("12. View Details\n");
        printf("13. Search Crew Member by ID\n");

        printf("14. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewCrewLogs();
                break;
            case 2:
                checkResources();
                break;
            case 3:
                viewPersonalInfo();
                break;
            case 4:
                viewMissionDetails();
                break;
            case 5:
                registerCrewMember();
                break;
            case 6:
                deleteCrewMember();
                break;
            case 7:
                checkSupplies();
                break;
            case 8:
                faultDetection();
                break;
            case 9:
                calculateTrajectory();
                break;
            case 10:
                addDetails();
                break;
            case 11:
                adminChoice = authenticateAdmin();
                if (adminChoice) {
                    printf("Admin options:\n");

                }
                break;
                case 12:
    viewDetails();
    break;

    case 13:
                color(10);
                printf("\nEnter Crew ID to search: ");
                color(15);
                scanf("%d", &id);
                searchCrewMemberByID(id);
                break;
            case 14:
                color(14);
                printf(" Exiting...\n");
                color(15);
                break;
            default:
                color(12);
                printf(" Invalid option, try again.\n");
                color(15);
        }
    } while (choice != 15);

    return 0;
}
