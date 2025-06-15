#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#define MAX_LINES 150
#define MAX_LINE_LENGTH 512

const int INDEL_COST = -5;

/**
 * @brief Function to remove spaces and tabs from a string
 * @param s The input string
 * @return A new string with all spaces and tabs removed
 */
const std::string removeSpace(const std::string& s) {
    std::string noSpace;
    for (int i = 0; i < s.size();++i)
        if (s[i] != ' ' && s[i] != '\t')
            noSpace.append(s[i] + "");

    return noSpace;
}

/**
 * @brief Function to calculate the score for two lines based on their content
 * @param line1 First line to compare
 * @param line2 Second line to compare
 * @return An integer score based on the comparison
 */
int calculateScore(const std::string& line1, const std::string& line2) {
    std::string line1NoSpace = removeSpace(line1);
    std::string line2NoSpace = removeSpace(line2);

    // Identifica se una riga contiene una graffa di apertura o chiusura
    bool l1_has_brace = (line1NoSpace.find("{") != std::string::npos) || (line1NoSpace.find("}") != std::string::npos);
    bool l2_has_brace = (line2NoSpace.find("{") != std::string::npos) || (line2NoSpace.find("}") != std::string::npos);

    // Caso 1: Le righe sono identiche (ignoring whitespace)
    if (line1NoSpace.compare(line2NoSpace) == 0) {
        if (l1_has_brace) 
            return 30; // Peso maggiore per match di struttura
        else 
            return 10; // Match normale
    }

    // Caso 2: Entrambe le righe contengono graffe (anche se il contenuto è diverso)
    if (l1_has_brace && l2_has_brace) 
        return 20; // Punteggio per match di "struttura di blocco"

    // Caso 3: Solo una delle due righe contiene una graffa
    if (l1_has_brace != l2_has_brace) 
        return -30; // Forte penalità per disallineamento di blocchi
    
    // Caso 4: Nessun match, nessuna graffa, semplicemente due righe diverse
    return -10; 
}


/**
 * @brief Function to align two files line by line using the Needleman-Wunsch algorithm
 * @param lines1 Array of strings representing the first file's lines
 * @param lines2 Array of strings representing the second file's lines
 * @param n1 Number of lines in the first file
 * @param n2 Number of lines in the second file
 * @param M Matrix for storing scores
 * @param P Matrix for storing types of moves (0 for up, 1 for left, 2 for diagonal)
 */
void align(const std::string* lines1, const std::string* lines2, int n1, int n2, int** M, char** P) {
    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            int score = calculateScore(lines1[i - 1], lines2[j - 1]);

            int score_diag = M[i - 1][j - 1] + score;
            int score_up = M[i - 1][j] + INDEL_COST;
            int score_left = M[i][j - 1] + INDEL_COST;

            if (score_diag >= score_up && score_diag >= score_left) {
                M[i][j] = score_diag;
                P[i][j] = 2; // Diagonal (match or mismatch)
            }
            else if (score_up >= score_left) {
                M[i][j] = score_up;
                P[i][j] = 0; // Up (deletion in file1)
            }
            else {
                M[i][j] = score_left;
                P[i][j] = 1; // Left (insertion in file2)
            }
        }
    }
}

/*

    fp1 = fopen(file1Path, "r");
    if (fp1 == NULL) {
        printf("Errore: Impossibile aprire il file %s\n", file1Path);
        return;
    }
    printf("Lettura di %s...\n", file1Path);
    while (numLines1 < MAX_LINES && fgets(buffer, MAX_LINE_LENGTH, fp1) != NULL) {
        std::string current_line = buffer;
        size_t newline_pos = current_line.find('\n');
        if (newline_pos != std::string::npos) {
            current_line.erase(newline_pos);
        }
        lines1[numLines1] = current_line;
        numLines1++;
    }
    fclose(fp1);
*/

/**
 * @brief Function to read a file and store its lines in an array of strings
 * @param filePath Path to the file to read
 * @param lines Array of strings to store the lines
 * @param numLines Reference to an integer to store the number of lines read
 */
void readFile(const char* filePath, std::string* lines, int& numLines) {
    FILE* fp = fopen(filePath, "r");
    if (fp == NULL) {
        numLines = -1; // Set numLines to -1 to indicate an error
        return;
    }
    char buffer[MAX_LINE_LENGTH]; // Buffer C-style per fgets
    while (numLines < MAX_LINES && fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) { // Read line by line until MAX_LINES or EOF
        std::string current_line = buffer;
        size_t newline_pos = current_line.find('\n');
        if (newline_pos != std::string::npos) {
            current_line.erase(newline_pos);
        }
        lines[numLines] = current_line;
        numLines++;
    }
    fclose(fp);
}

/**
 * @brief Function to perform the Needleman-Wunsch algorithm for file comparison
 * @param file1Path Path to the first file
 * @param file2Path Path to the second file
 */
void needleman(const char* file1Path, const char* file2Path) {
    std::string lines1[MAX_LINES];
    std::string lines2[MAX_LINES];
    int numLines1 = 0;
    int numLines2 = 0;

    FILE* fp1;
    FILE* fp2;
    char buffer[MAX_LINE_LENGTH]; // Buffer C-style per fgets

    // File reading 
    readFile(file1Path, lines1, numLines1);
    if (numLines1 == -1) {
        printf("Errore nella lettura del file %s\n", file1Path);
        return; // Exit if there was an error reading the first file
    }
    readFile(file2Path, lines2, numLines2);
    if (numLines2 == -1) {
        printf("Errore nella lettura del file %s\n", file2Path);
        return; // Exit if there was an error reading the second file
    }

    printf("File 1 ha %d linee, File 2 ha %d linee.\n", numLines1, numLines2);


    // Inizializzazione delle matrici di programmazione dinamica 
    int** M = new int*[numLines1 + 1];
    char** P = new char*[numLines1 + 1];
    for (int i = 0; i <= numLines1; ++i) {
        M[i] = new int[numLines2 + 1];
        P[i] = new char[numLines2 + 1];
    }

    // Inizializza la prima riga e la prima colonna della matrice M
    for (int i = 0; i <= numLines1; ++i) {
        M[i][0] = i * INDEL_COST;
        P[i][0] = 0; // Vengo da sopra (cancellazione in file1)
    }
    for (int j = 0; j <= numLines2; ++j) {
        M[0][j] = j * INDEL_COST;
        P[0][j] = 1; // Vengo da sinistra (inserzione in file2)
    }
    P[0][0] = -1; // Nessuna provenienza per la cella iniziale


    printf("Calcolo della matrice di allineamento...\n");
    align(lines1, lines2, numLines1, numLines2, M, P);


    // --- Ricostruzione e stampa dell'allineamento ---
    printf("\n--- Differenze tra i file ---\n");
    printf("Legenda: (+) Aggiunta, (-) Rimozione, ( ) Nessuna modifica, (~) Modifica\n\n");

    int i = numLines1;
    int j = numLines2;
    std::string output_file1 = "";
    std::string output_file2 = "";

    std::string aligned_lines1[MAX_LINES + MAX_LINES]; // Abbastanza grande per tutti i casi
    std::string aligned_lines2[MAX_LINES + MAX_LINES];
    int current_aligned_idx = 0; // Indice per riempire gli array allineati

    while (i > 0 || j > 0) {
        if (P[i][j] == 2) { // Vengo dalla diagonale (Match o Mismatch/Modifica)
            std::string trimmed_line1_check = removeSpace(lines1[i - 1]);
            std::string trimmed_line2_check = removeSpace(lines2[j - 1]);

            if (trimmed_line1_check == trimmed_line2_check) {
                // Le righe sono identiche (dopo rimozione spazi)
                aligned_lines1[current_aligned_idx] = "  " + lines1[i - 1]; // Spazio per indentazione
                aligned_lines2[current_aligned_idx] = "  " + lines2[j - 1];
            } else {
                // Le righe sono diverse, è un Mismatch / Modifica
                aligned_lines1[current_aligned_idx] = "~ " + lines1[i - 1];
                aligned_lines2[current_aligned_idx] = "~ " + lines2[j - 1];
            }
            i--; j--;
        } else if (P[i][j] == 0) { // Vengo da sopra (Rimozione da file1)
            aligned_lines1[current_aligned_idx] = "- " + lines1[i - 1];
            aligned_lines2[current_aligned_idx] = "  "; // Riga vuota per allineamento visivo
            i--;
        } else { // P[i][j] == 1 // Vengo da sinistra (Inserimento in file2)
            aligned_lines1[current_aligned_idx] = "  "; // Riga vuota per allineamento visivo
            aligned_lines2[current_aligned_idx] = "+ " + lines2[j - 1];
            j--;
        }
        current_aligned_idx++;
    }

    // Stampa delle linee allineate in ordine corretto 
    printf("File 1                             | File 2\n");
    printf("-----------------------------------|-----------------------------------\n");

    for (int k = current_aligned_idx - 1; k >= 0; --k) {
        std::string s1 = aligned_lines1[k];
        std::string s2 = aligned_lines2[k];
        
        if (s1.length() > 35) s1 = s1.substr(0, 32) + "...";
        if (s2.length() > 35) s2 = s2.substr(0, 32) + "...";
        
        printf("%-35s| %s\n", s1.c_str(), s2.c_str());
    }

    // --- Deallocazione della memoria delle matrici M e P ---
    for (int k = 0; k <= numLines1; ++k) {
        delete[] M[k];
        delete[] P[k];
    }
    delete[] M;
    delete[] P;
}

int main() {
    const char* filePath1 = "file1.cpp";
    const char* filePath2 = "file2.cpp";

    needleman(filePath1, filePath2);
}