#include <bits/stdc++.h>
using namespace std;

int main() {
  {
    // table for exp1
    ofstream table1("exp1avg.txt");
    table1 << "\\begin{center}" << "\n";
    table1
        << "\\resizebox{\\textwidth}{!}{\\begin{tabular}{ |c|c|c|c|c|c|c|c|c|}"
        << "\n";
    table1 << "\\hline" << "\n";
    table1 << "s No. & x-axis & TAS Avg CS & CAS Avg CS & "
              "Bounded CAS Avg & TAS Avg CS  & CAS Avg "
              " & Bounded CAS Avg & Total Time\\\\"
           << "\n";
    table1 << " &  & Entry Time & Entry Time & "
              " CS Entry Time & Exit Time &"
              "CS Exit Time & CS Exit Time & Taken\\\\"
           << "\n";
    table1 << "\\hline" << "\n";
    ofstream table2("exp1w.txt");
    table2 << "\\begin{center}" << "\n";
    table2
        << "\\resizebox{\\textwidth}{!}{\\begin{tabular}{ |c|c|c|c|c|c|c|c|c|}"
        << "\n";
    table2 << "\\hline" << "\n";
    table2 << "s No. & x-axis & TAS worst CS & CAS worst CS & "
              "Bounded CAS worst & TAS worst CS  & CAS worst"
              " & Bounded CAS worst & Total Time\\\\"
           << "\n";
    table2 << " &  & Entry Time & Entry Time & "
              " CS Entry Time & Exit Time &"
              "CS Exit Time & CS Exit Time & Taken\\\\"
           << "\n";
    table2 << "\\hline" << "\n";
    int slno = 0;
    ofstream timings("timesexp1.txt");
    for (int i = 20; i <= 100; i += 10) {
      slno++;
      string filename =
          "../tests/times/valid_exp1_" + to_string(i * i) + ".txt";
      ifstream file1(filename);
      int xaxis = i * i;
      double totaltime = 0;
      double avgTas = 0;
      double avgCas = 0;
      double avgBas = 0;
      double avgTasx = 0;
      double avgCasx = 0;
      double avgBasx = 0;
      double wTas = 0;
      double wCas = 0;
      double wBas = 0;
      double wTasx = 0;
      double wCasx = 0;
      double wBasx = 0;
      double tTas = 0;
      double tCas = 0;
      double tBas = 0;

      double dummy;
      for (int j = 0; j < 5; j++) {
        file1 >> dummy;
        totaltime += dummy;
        file1 >> dummy;
        avgTas += dummy;
        file1 >> dummy;
        avgTasx += dummy;
        file1 >> dummy;
        wTas += dummy;
        file1 >> dummy;
        wTasx += dummy;
        file1 >> dummy;
        tTas += dummy;

        file1 >> dummy;
        avgCas += dummy;
        file1 >> dummy;
        avgCasx += dummy;
        file1 >> dummy;
        wCas += dummy;
        file1 >> dummy;
        wCasx += dummy;
        file1 >> dummy;
        tCas += dummy;

        file1 >> dummy;
        avgBas += dummy;
        file1 >> dummy;
        avgBasx += dummy;
        file1 >> dummy;
        wBas += dummy;
        file1 >> dummy;
        wBasx += dummy;
        file1 >> dummy;
        tBas += dummy;
      }
      file1.close();
      string filename2 =
          "../tests/times/invalid_exp1_" + to_string(i * i) + ".txt";
      ifstream file2(filename2);
      for (int j = 0; j < 5; j++) {
        file2 >> dummy;
        totaltime += dummy;
        file2 >> dummy;
        avgTas += dummy;
        file2 >> dummy;
        avgTasx += dummy;
        file2 >> dummy;
        wTas += dummy;
        file2 >> dummy;
        wTasx += dummy;
        file2 >> dummy;
        tTas += dummy;

        file2 >> dummy;
        avgCas += dummy;
        file2 >> dummy;
        avgCasx += dummy;
        file2 >> dummy;
        wCas += dummy;
        file2 >> dummy;
        wCasx += dummy;
        file2 >> dummy;
        tCas += dummy;

        file2 >> dummy;
        avgBas += dummy;
        file2 >> dummy;
        avgBasx += dummy;
        file2 >> dummy;
        wBas += dummy;
        file2 >> dummy;
        wBasx += dummy;
        file2 >> dummy;
        tBas += dummy;
      }
      file2.close();
      totaltime /= 10;
      avgBasx /= 10;
      avgBas /= 10;
      avgCas /= 10;
      avgCasx /= 10;
      avgTas /= 10;
      avgTasx /= 10;
      wTas /= 10;
      wTasx /= 10;
      wCas /= 10;
      wCasx /= 10;
      wBas /= 10;
      wBasx /= 10;
      tTas /= 10;
      tCas /= 10;
      tBas /= 10;
      table1 << slno << " & " << xaxis << " & " << avgTas << " & " << avgCas
             << " & " << avgBas << " & " << avgTasx << " & " << avgCasx << " & "
             << avgBasx << " & " << totaltime + tTas + tBas + tCas << "\\\\"
             << "\n";
      table2 << slno << " & " << xaxis << " & " << wTas << " & " << wCas
             << " & " << wBas << " & " << wTasx << " & " << wCasx << " & "
             << wBasx << " & " << totaltime + tTas + tBas + tCas << "\\\\"
             << "\n";
      timings << xaxis << " " << totaltime << " " << tTas << " " << tCas << " "
              << tBas << "\n";
    }
    timings.close();
    table1 << "\\hline" << "\n";
    table2 << "\\hline" << "\n";
    table1 << "\\end{tabular}}" << "\n";
    table1 << "\\end{center}" << "\n";
    table1.close();
    table2 << "\\end{tabular}}" << "\n";
    table2 << "\\end{center}" << "\n";
    table2.close();
  }
  {
    // table for exp2
    ofstream table1("exp2avg.txt");
    table1 << "\\begin{center}" << "\n";
    table1
        << "\\resizebox{\\textwidth}{!}{\\begin{tabular}{ |c|c|c|c|c|c|c|c|c|}"
        << "\n";
    table1 << "\\hline" << "\n";
    table1 << "s No. & x-axis & TAS Avg CS & CAS Avg CS & "
              "Bounded CAS Avg & TAS Avg CS  & CAS Avg "
              " & Bounded CAS Avg & Total Time\\\\"
           << "\n";
    table1 << " &  & Entry Time & Entry Time & "
              " CS Entry Time & Exit Time &"
              "CS Exit Time & CS Exit Time & Taken\\\\"
           << "\n";
    table1 << "\\hline" << "\n";
    ofstream table2("exp2w.txt");
    table2 << "\\begin{center}" << "\n";
    table2
        << "\\resizebox{\\textwidth}{!}{\\begin{tabular}{ |c|c|c|c|c|c|c|c|c|}"
        << "\n";
    table2 << "\\hline" << "\n";
    table2 << "s No. & x-axis & TAS worst CS & CAS worst CS & "
              "Bounded CAS worst & TAS worst CS  & CAS worst"
              " & Bounded CAS worst & Total Time\\\\"
           << "\n";
    table2 << " &  & Entry Time & Entry Time & "
              " CS Entry Time & Exit Time &"
              "CS Exit Time & CS Exit Time & Taken\\\\"
           << "\n";
    table2 << "\\hline" << "\n";
    int slno = 0;
    ofstream timings("timesexp2.txt");
    for (int i = 10; i <= 50; i += 5) {
      slno++;
      string filename = "../tests/times/valid_exp2_" + to_string(i) + ".txt";
      ifstream file1(filename);
      int xaxis = i;
      double totaltime = 0;
      double avgTas = 0;
      double avgCas = 0;
      double avgBas = 0;
      double avgTasx = 0;
      double avgCasx = 0;
      double avgBasx = 0;
      double wTas = 0;
      double wCas = 0;
      double wBas = 0;
      double wTasx = 0;
      double wCasx = 0;
      double wBasx = 0;
      double tTas = 0;
      double tCas = 0;
      double tBas = 0;

      double dummy;
      for (int j = 0; j < 5; j++) {
        file1 >> dummy;
        totaltime += dummy;
        file1 >> dummy;
        avgTas += dummy;
        file1 >> dummy;
        avgTasx += dummy;
        file1 >> dummy;
        wTas += dummy;
        file1 >> dummy;
        wTasx += dummy;
        file1 >> dummy;
        tTas += dummy;

        file1 >> dummy;
        avgCas += dummy;
        file1 >> dummy;
        avgCasx += dummy;
        file1 >> dummy;
        wCas += dummy;
        file1 >> dummy;
        wCasx += dummy;
        file1 >> dummy;
        tCas += dummy;

        file1 >> dummy;
        avgBas += dummy;
        file1 >> dummy;
        avgBasx += dummy;
        file1 >> dummy;
        wBas += dummy;
        file1 >> dummy;
        wBasx += dummy;
        file1 >> dummy;
        tBas += dummy;
      }
      file1.close();
      string filename2 = "../tests/times/invalid_exp2_" + to_string(i) + ".txt";
      ifstream file2(filename2);
      for (int j = 0; j < 5; j++) {
        file2 >> dummy;
        totaltime += dummy;
        file2 >> dummy;
        avgTas += dummy;
        file2 >> dummy;
        avgTasx += dummy;
        file2 >> dummy;
        wTas += dummy;
        file2 >> dummy;
        wTasx += dummy;
        file2 >> dummy;
        tTas += dummy;

        file2 >> dummy;
        avgCas += dummy;
        file2 >> dummy;
        avgCasx += dummy;
        file2 >> dummy;
        wCas += dummy;
        file2 >> dummy;
        wCasx += dummy;
        file2 >> dummy;
        tCas += dummy;

        file2 >> dummy;
        avgBas += dummy;
        file2 >> dummy;
        avgBasx += dummy;
        file2 >> dummy;
        wBas += dummy;
        file2 >> dummy;
        wBasx += dummy;
        file2 >> dummy;
        tBas += dummy;
      }
      file2.close();
      totaltime /= 10;
      avgBasx /= 10;
      avgBas /= 10;
      avgCas /= 10;
      avgCasx /= 10;
      avgTas /= 10;
      avgTasx /= 10;
      wTas /= 10;
      wTasx /= 10;
      wCas /= 10;
      wCasx /= 10;
      wBas /= 10;
      wBasx /= 10;
      tTas /= 10;
      tCas /= 10;
      tBas /= 10;
      table1 << slno << " & " << xaxis << " & " << avgTas << " & " << avgCas
             << " & " << avgBas << " & " << avgTasx << " & " << avgCasx << " & "
             << avgBasx << " & " << totaltime + tTas + tBas + tCas << "\\\\"
             << "\n";
      table2 << slno << " & " << xaxis << " & " << wTas << " & " << wCas
             << " & " << wBas << " & " << wTasx << " & " << wCasx << " & "
             << wBasx << " & " << totaltime + tTas + tBas + tCas << "\\\\"
             << "\n";
      timings << xaxis << " " << totaltime << " " << tTas << " " << tCas << " "
              << tBas << "\n";
    }
    timings.close();
    table1 << "\\hline" << "\n";
    table2 << "\\hline" << "\n";
    table1 << "\\end{tabular}}" << "\n";
    table1 << "\\end{center}" << "\n";
    table1.close();
    table2 << "\\end{tabular}}" << "\n";
    table2 << "\\end{center}" << "\n";
    table2.close();
  }
  {
    // table for exp3
    ofstream table1("exp3avg.txt");
    table1 << "\\begin{center}" << "\n";
    table1
        << "\\resizebox{\\textwidth}{!}{\\begin{tabular}{ |c|c|c|c|c|c|c|c|c|}"
        << "\n";
    table1 << "\\hline" << "\n";
    table1 << "s No. & x-axis & TAS Avg CS & CAS Avg CS & "
              "Bounded CAS Avg & TAS Avg CS  & CAS Avg "
              " & Bounded CAS Avg & Total Time\\\\"
           << "\n";
    table1 << " &  & Entry Time & Entry Time & "
              " CS Entry Time & Exit Time &"
              "CS Exit Time & CS Exit Time & Taken\\\\"
           << "\n";
    table1 << "\\hline" << "\n";
    ofstream table2("exp3w.txt");
    table2 << "\\begin{center}" << "\n";
    table2
        << "\\resizebox{\\textwidth}{!}{\\begin{tabular}{ |c|c|c|c|c|c|c|c|c|}"
        << "\n";
    table2 << "\\hline" << "\n";
    table2 << "s No. & x-axis & TAS worst CS & CAS worst CS & "
              "Bounded CAS worst & TAS worst CS  & CAS worst"
              " & Bounded CAS worst & Total Time\\\\"
           << "\n";
    table2 << " &  & Entry Time & Entry Time & "
              " CS Entry Time & Exit Time &"
              "CS Exit Time & CS Exit Time & Taken\\\\"
           << "\n";
    table2 << "\\hline" << "\n";
    int slno = 0;
    ofstream timings("timesexp3.txt");
    for (int i = 1; i <= 32; i <<= 1) {
      slno++;
      string filename = "../tests/times/valid_exp3_" + to_string(i) + ".0.txt";
      ifstream file1(filename);
      int xaxis = i;
      double totaltime = 0;
      double avgTas = 0;
      double avgCas = 0;
      double avgBas = 0;
      double avgTasx = 0;
      double avgCasx = 0;
      double avgBasx = 0;
      double wTas = 0;
      double wCas = 0;
      double wBas = 0;
      double wTasx = 0;
      double wCasx = 0;
      double wBasx = 0;
      double tTas = 0;
      double tCas = 0;
      double tBas = 0;

      double dummy;
      for (int j = 0; j < 5; j++) {
        file1 >> dummy;
        totaltime += dummy;
        file1 >> dummy;
        avgTas += dummy;
        file1 >> dummy;
        avgTasx += dummy;
        file1 >> dummy;
        wTas += dummy;
        file1 >> dummy;
        wTasx += dummy;
        file1 >> dummy;
        tTas += dummy;

        file1 >> dummy;
        avgCas += dummy;
        file1 >> dummy;
        avgCasx += dummy;
        file1 >> dummy;
        wCas += dummy;
        file1 >> dummy;
        wCasx += dummy;
        file1 >> dummy;
        tCas += dummy;

        file1 >> dummy;
        avgBas += dummy;
        file1 >> dummy;
        avgBasx += dummy;
        file1 >> dummy;
        wBas += dummy;
        file1 >> dummy;
        wBasx += dummy;
        file1 >> dummy;
        tBas += dummy;
      }
      file1.close();
      string filename2 =
          "../tests/times/invalid_exp3_" + to_string(i) + ".0.txt";
      ifstream file2(filename2);
      for (int j = 0; j < 5; j++) {
        file2 >> dummy;
        totaltime += dummy;
        file2 >> dummy;
        avgTas += dummy;
        file2 >> dummy;
        avgTasx += dummy;
        file2 >> dummy;
        wTas += dummy;
        file2 >> dummy;
        wTasx += dummy;
        file2 >> dummy;
        tTas += dummy;

        file2 >> dummy;
        avgCas += dummy;
        file2 >> dummy;
        avgCasx += dummy;
        file2 >> dummy;
        wCas += dummy;
        file2 >> dummy;
        wCasx += dummy;
        file2 >> dummy;
        tCas += dummy;

        file2 >> dummy;
        avgBas += dummy;
        file2 >> dummy;
        avgBasx += dummy;
        file2 >> dummy;
        wBas += dummy;
        file2 >> dummy;
        wBasx += dummy;
        file2 >> dummy;
        tBas += dummy;
      }
      file2.close();
      totaltime /= 10;
      avgBasx /= 10;
      avgBas /= 10;
      avgCas /= 10;
      avgCasx /= 10;
      avgTas /= 10;
      avgTasx /= 10;
      wTas /= 10;
      wTasx /= 10;
      wCas /= 10;
      wCasx /= 10;
      wBas /= 10;
      wBasx /= 10;
      tTas /= 10;
      tCas /= 10;
      tBas /= 10;
      table1 << slno << " & " << xaxis << " & " << avgTas << " & " << avgCas
             << " & " << avgBas << " & " << avgTasx << " & " << avgCasx << " & "
             << avgBasx << " & " << totaltime + tTas + tBas + tCas << "\\\\"
             << "\n";
      table2 << slno << " & " << xaxis << " & " << wTas << " & " << wCas
             << " & " << wBas << " & " << wTasx << " & " << wCasx << " & "
             << wBasx << " & " << totaltime + tTas + tBas + tCas << "\\\\"
             << "\n";
      timings << xaxis << " " << totaltime << " " << tTas << " " << tCas << " "
              << tBas << "\n";
    }
    timings.close();
    table1 << "\\hline" << "\n";
    table2 << "\\hline" << "\n";
    table1 << "\\end{tabular}}" << "\n";
    table1 << "\\end{center}" << "\n";
    table1.close();
    table2 << "\\end{tabular}}" << "\n";
    table2 << "\\end{center}" << "\n";
    table2.close();
  }
  return 0;
}
