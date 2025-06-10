void align_source_code(const char* file1_path, const char* file2_path) {
  // Usiamo array C-style di std::string per le linee
  std::string lines1[MAX_LINES];
  std::string lines2[MAX_LINES];
  int num_lines1 = 0;
  int num_lines2 = 0;

  FILE* fp1;
  FILE* fp2;
  char buffer[MAX_LINE_LENGTH]; // Buffer C-style per fgets

  // --- Lettura del File 1 ---   RIV1
  fp1 = fopen(file1_path, "r");
  if (fp1 == NULL) {
    printf("Errore: Impossibile aprire il file %s\n", file1_path);
    return;
  }
  printf("Lettura di %s...\n", file1_path);
  while (num_lines1 < MAX_LINES && fgets(buffer, MAX_LINE_LENGTH, fp1) != NULL) {
    std::string current_line = buffer;
    size_t newline_pos = current_line.find('\n');
    if (newline_pos != std::string::npos) {
      current_line.erase(newline_pos);
    }
    lines1[num_lines1] = current_line;
    num_lines1++;
  }
  fclose(fp1);

  // --- Lettura del File 2 ---   RIV1
  fp2 = fopen(file2_path, "r");
  if (fp2 == NULL) {
    printf("Errore: Impossibile aprire il file %s\n", file2_path);
    return;
  }
  printf("Lettura di %s...\n", file2_path);
  while (num_lines2 < MAX_LINES && fgets(buffer, MAX_LINE_LENGTH, fp2) != NULL) {
    std::string current_line = buffer;
    size_t newline_pos = current_line.find('\n');
    if (newline_pos != std::string::npos) {
      current_line.erase(newline_pos);
    }
    lines2[num_lines2] = current_line;
    num_lines2++;
  }
  fclose(fp2);

  printf("File 1 ha %d linee, File 2 ha %d linee.\n", num_lines1, num_lines2);

  // --- Inizializzazione delle matrici di programmazione dinamica --- RIV2
  M = (int*)malloc((num_lines1 + 1) * sizeof(int));
  P = (char*)malloc((num_lines1 + 1) * sizeof(char));
  for (int i = 0; i <= num_lines1; ++i) {
    M[i] = (int*)malloc((num_lines2 + 1) * sizeof(int));
    P[i] = (char*)malloc((num_lines2 + 1) * sizeof(char));
  }

  // Inizializza la prima riga e la prima colonna della matrice M
  for (int i = 0; i <= num_lines1; ++i) {
    M[i][0] = i * INDEL_COST;
    P[i][0] = 0; // Vengo da sopra (cancellazione in file1)
  }
  for (int j = 0; j <= num_lines2; ++j) {
    M[0][j] = j * INDEL_COST;
    P[0][j] = 1; // Vengo da sinistra (inserzione in file2)
  }
  P[0][0] = -1; // Nessuna provenienza per la cella iniziale

  // --- Riempimento delle matrici M e P ---
  printf("Calcolo della matrice di allineamento...\n");
  for (int i = 1; i <= num_lines1; ++i) {
    for (int j = 1; j <= num_lines2; ++j) {
      int score = calculate_score(lines1[i - 1], lines2[j - 1]);

      int score_diag = M[i - 1][j - 1] + score;
      int score_up = M[i - 1][j] + INDEL_COST;
      int score_left = M[i][j - 1] + INDEL_COST;

      if (score_diag >= score_up && score_diag >= score_left) {
        M[i][j] = score_diag;
        P[i][j] = 2; // Vengo da diagonale
      }
      else if (score_up >= score_left) {
        M[i][j] = score_up;
        P[i][j] = 0; // Vengo da sopra
      }
      else {
        M[i][j] = score_left;
        P[i][j] = 1; // Vengo da sinistra
      }
    }
  }

  // --- Ricostruzione e stampa dell'allineamento --- RIV1
  printf("\n--- Differenze tra i file ---\n");
  printf("Legenda: (+) Aggiunta, (-) Rimozione, ( ) Nessuna modifica, (~) Modifica\n\n");

  int i = num_lines1;
  int j = num_lines2;
  std::string output_file1 = "";
  std::string output_file2 = "";

  // Questo è un approccio semplificato per la ricostruzione.
  // Per un output "diff" più fedele, dovremmo usare un algoritmo di backtracking
  // che riproduca esattamente le modifiche.
  // L'algoritmo attuale identifica le modifiche ma la visualizzazione è basilare.

  while (i > 0 || j > 0) {
    if (P[i][j] == 2) { // Match o Mismatch
      // Ignoriamo la spaziatura per il confronto, ma stampiamo l'originale
      std::string trimmed_line1_check;
      for (char c : lines1[i - 1]) { if (!is_whitespace(c)) trimmed_line1_check += c; }
      std::string trimmed_line2_check;
      for (char c : lines2[j - 1]) { if (!is_whitespace(c)) trimmed_line2_check += c; }

      if (trimmed_line1_check != trimmed_line2_check) { // Linea modificata (non solo whitespace)
        output_file1 = "~ " + lines1[i - 1] + "\n" + output_file1;
        output_file2 = "~ " + lines2[j - 1] + "\n" + output_file2;
      }
      else { // Linea identica (anche con whitespace diversi, se ignrati nel score)
        output_file1 = "  " + lines1[i - 1] + "\n" + output_file1;
        output_file2 = "  " + lines2[j - 1] + "\n" + output_file2;
      }
      i--; j--;
    }
    else if (P[i][j] == 0) { // Linea rimossa (solo in file1)
      output_file1 = "- " + lines1[i - 1] + "\n" + output_file1;
      output_file2 = "  (rimossa)\n" + output_file2; // Placeholder per allineamento
      i--;
    }
    else { // P[i][j] == 1 // Linea aggiunta (solo in file2)
      output_file1 = "  (aggiunta)\n" + output_file1; // Placeholder per allineamento
      output_file2 = "+ " + lines2[j - 1] + "\n" + output_file2;
      j--;
    }
  }

  printf("File 1:\n%s", output_file1.c_str());
  printf("\nFile 2:\n%s", output_file2.c_str());

  // --- Deallocazione della memoria delle matrici M e P ---
  for (int k = 0; k <= num_lines1; ++k) {
    free(M[k]);
    free(P[k]);
  }
  free(M);
  free(P);
}