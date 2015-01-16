#include <iostream>
#include <string>
#include <stdio.h>
#include <QApplication>
#include <QClipboard>
#include <QString>

using namespace std;

string get_decrypted() {
  char *cmd = new char[1024]();
  strcpy(cmd,"ccrypt -c /home/lerignoux/Perso/Pass.yml.cpt");
  FILE* pipe = popen(cmd, "r");
  if (!pipe) return "ERROR";
  char buffer[128];
  string result = "";
  while(!feof(pipe)) {
    if(fgets(buffer, 128, pipe) != NULL)
      result += buffer;
  }
  pclose(pipe);
  return result;
}

string select_group(string& full, const string& search) {
  size_t pos = full.find(search);
  if (pos==string::npos) {
    cout << "Password not found cleaning up." << endl;
    full.replace(0, full.length(), full.length(), '0');
    full.erase();
  }
  else {
    pos = pos + search.length();
    full.replace(0, pos, pos, '0');
    full.erase(0, pos);
    size_t end = full.find("\n\n");
    if (end < full.length()) {
      full.replace(end, full.length(), (full.length() - end) ,'0');
      full.erase(end);
    }
  }
  return full;
}


string get_password(string& pass) {
  string full = get_decrypted();
  string group = select_group(full, pass);
  return select_group(group, "pass: ");
}


int main(int argc, char *argv[])
{
  if (argc == 0) return 1;
  QApplication app(argc, argv);
  QString password;
  string pass = string(argv[1]) + string(":\n");
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setTextFormat(Qt::PlainText);
  cout << clipboard->text().toStdString() << endl;
  password = QString::fromStdString(get_password(pass));
  clipboard->setText(password, QClipboard::Clipboard);
  cout << clipboard->text().toStdString() << endl;
  // clipBoard->setText(password, QClipboard::Selection);
  return 0;
}
