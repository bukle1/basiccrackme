// main.cpp
// KEY, GitHub RAW dosyasından okunur.
// KEY doğruysa cracked_note.txt oluşturulur ve otomatik açılır.
// CMD ekranında başarı mesajı yazılır.

#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <cstdio>
#pragma comment(lib, "wininet.lib")

std::string DownloadText(const char* url)
{
    HINTERNET hI = InternetOpenA("KeyChecker", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hI) return "";

    HINTERNET hF = InternetOpenUrlA(hI, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hF)
    {
        InternetCloseHandle(hI);
        return "";
    }

    char buffer[2048];
    DWORD bytesRead = 0;
    std::string result = "";

    while (InternetReadFile(hF, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        result.append(buffer, bytesRead);
    }

    InternetCloseHandle(hF);
    InternetCloseHandle(hI);

    return result;
}

int main()
{
    std::cout << "=== Key Checker ===\n";

    const char* url = "https://raw.githubusercontent.com/bukle1/versionchecker/refs/heads/main/version.txt";

    std::cout << "Online key okunuyor...\n";

    std::string onlineKey = DownloadText(url);

    if (onlineKey.empty())
    {
        std::cout << "HATA: Key dosyasi indirilemedi!\n";
        std::cout << "ENTER'a bas...";
        std::cin.get();
        return 1;
    }

    while (!onlineKey.empty() && (onlineKey.back() == '\n' || onlineKey.back() == '\r' || onlineKey.back() == ' '))
        onlineKey.pop_back();

    std::cout << "Key giriniz: ";
    std::string myKey;
    std::getline(std::cin, myKey);

    if (myKey == onlineKey)
    {
        std::cout << "Giris basarili!\n";

        FILE* f;
        fopen_s(&f, "cracked_note.txt", "w");
        if (f)
        {
            fprintf(f, "Key Dogrulama Basarili!\\nKey: %s\\n", myKey.c_str());
            fclose(f);
        }

        // Not defterini otomatik aç
        ShellExecuteA(NULL, "open", "cracked_note.txt", NULL, NULL, SW_SHOWNORMAL);

        std::cout << "Not dosyasi olusturuldu ve acildi!\n";
    }
    else
    {
        std::cout << "Hatali key!\n";
    }

    std::cout << "\nENTER'a basarak cik...";
    std::cin.get();
    return 0;
}
