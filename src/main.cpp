#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
void analyzeContract(const string& filename) {
    // Open the target Solidity contract file safely using file streams//
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }
string line;
int riskScore = 0;
vector<string> findings;
bool inBlockComment = false; // Tracks whether we're inside a /* ... */ block that spans multiple lines
while (getline(file, line)) {
        line = line.substr(0, line.find("//")); // Strip "//" comments so comment text isn't scanned as code
string code;
       for (size_t i = 0; i < line.size(); ++i) {
            if (inBlockComment) {
                if (line.compare(i, 2, "*/") == 0) { inBlockComment = false; 
                 i++; }
            } else if (line.compare(i, 2, "/*") == 0) {
                inBlockComment = true; i++;
            } else {
                code += line[i];
            }
        }
           line = code; // From here on, "line" contains only real code, no comment text
        // Check for reentrancy pattern indicator : (unsafe external call before state update)
     
        if (line.find(".call{value:") != string::npos) {
            riskScore += 40;
            findings.push_back("[HIGH] Potential Reentrancy vector found (.call{value:...})");
        }
        // Check for unsafe tx.origin : Authentication Risk
        if (line.find("tx.origin") != string::npos) {
            riskScore += 30;
            findings.push_back("[MEDIUM] Use of 'tx.origin' detected for authentication (Phishing risk)");
        }
        // Check for assembly blocks
        if (line.find("assembly") != string::npos) {
            riskScore += 20;
            findings.push_back("[INFO] Inline assembly block found (Requires manual review)");
        }
    }

    file.close();

    cout << " Analyzing target: " << filename << endl;
    cout << "----------------------------------------" << endl;
    if (!findings.empty()) {
        for (const string& finding : findings) {
            cout << "  - " << finding << endl;
        }
    } else {
        // If no vulnerabilities were found, output a clean status message
        cout << "  - No obvious high-risk patterns detected." << endl;
    }
    cout << "----------------------------------------" << endl;
    // Output the final computed risk score out of 100
    cout << "Final Calculated Risk Score: " << riskScore << " / 100" << endl;
    
    if (riskScore >= 50) {
        cout << "Status:  HIGH RISK (Vulnerabilities found)" << endl;
    } else if (riskScore > 0) {
        cout << "Status:  MEDIUM RISK (Review recommended)" << endl;
    } else {
         // Evaluate total findings and output risk  (Low/Medium/High)
        cout << "Status:  LOW RISK" << endl;
       }
}
  //The program name itself counts as 1:argument count
int main(int argc, char* argv[]) {
   string target = "contract.sol";
    
    if (argc > 1) {
        target = argv[1];
    }
     analyzeContract(target);
    return 0;
}
