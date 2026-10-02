// =============================================================================
// student.cpp — Implementasi Mahasiswa
// Pertemuan 5: Stack (Tumpukan) dengan Linked List
// =============================================================================
// FILE YANG BOLEH DIEDIT      : src/student.cpp
// FILE YANG TIDAK BOLEH DIEDIT: src/student.h, tests/checker.cpp, tests/report.h
// =============================================================================

#include "student.h"

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// =============================================================================
// SUDAH DISEDIAKAN
// =============================================================================

void inisialisasi(Stack& s) {
    s.top = nullptr;
}

bool isEmpty(const Stack& s) {
    return s.top == nullptr;
}

bool peek(Stack& s, int& nilai) {
    if (s.top == nullptr)
        return false;

    nilai = s.top->data;
    return true;
}

string display(Stack& s) {
    string hasil;

    for (Node* p = s.top; p != nullptr; p = p->next) {
        if (!hasil.empty())
            hasil += " ";

        hasil += to_string(p->data);
    }

    return hasil;
}

// =============================================================================
// SOAL 1 — PUSH
// =============================================================================

bool push(Stack& s, int nilai) {
    Node* newNode = new Node;

    if (newNode == nullptr)
        return false;

    newNode->data = nilai;
    newNode->next = s.top;
    s.top = newNode;

    return true;
}

// =============================================================================
// SOAL 2 — POP
// =============================================================================

bool pop(Stack& s, int& nilai) {
    if (s.top == nullptr)
        return false;

    Node* temp = s.top;

    nilai = temp->data;

    s.top = s.top->next;

    delete temp;

    return true;
}

// =============================================================================
// SOAL 3 — CLEAR
// =============================================================================

void clear(Stack& s) {
    while (s.top != nullptr) {
        Node* del = s.top;

        s.top = s.top->next;

        delete del;
    }
}

// =============================================================================
// SOAL 4 — KURUNG SEIMBANG
// =============================================================================

bool kurungSeimbang(const string& ekspresi) {
    Node* top = nullptr;

    for (char c : ekspresi) {

        // Jika kurung buka, masukkan ke stack
        if (c == '(' || c == '[' || c == '{') {

            Node* baru = new Node;

            if (baru == nullptr) {
                while (top != nullptr) {
                    Node* del = top;
                    top = top->next;
                    delete del;
                }

                return false;
            }

            baru->data = c;
            baru->next = top;
            top = baru;
        }

        // Jika kurung tutup
        else if (c == ')' || c == ']' || c == '}') {

            // Tidak ada pasangan kurung buka
            if (top == nullptr) {
                return false;
            }

            char buka = static_cast<char>(top->data);

            // Kurung tidak cocok
            if ((c == ')' && buka != '(') ||
                (c == ']' && buka != '[') ||
                (c == '}' && buka != '{')) {

                while (top != nullptr) {
                    Node* del = top;
                    top = top->next;
                    delete del;
                }

                return false;
            }

            // Kurung cocok, keluarkan dari stack
            Node* del = top;
            top = top->next;
            delete del;
        }
    }

    bool seimbang = (top == nullptr);

    // Bersihkan sisa node
    while (top != nullptr) {
        Node* del = top;
        top = top->next;
        delete del;
    }

    return seimbang;
}

// =============================================================================
// MAIN() — TIDAK DINILAI
// =============================================================================

#ifndef ADA_MAIN_LAIN

static const char* benarSalah(bool nilai) {
    return nilai ? "true" : "false";
}

static ostream& baris(const string& label) {
    return cout << "    " << left << setw(20) << label << ": ";
}

// Menampilkan keadaan stack
static void keadaan(Stack& s) {
    baris("display") << "\"" << display(s) << "\"\n";

    int atas = 0;

    if (peek(s, atas))
        baris("puncak") << atas << "\n";
    else
        baris("puncak") << "(tidak ada)\n";

    baris("isEmpty") << benarSalah(isEmpty(s)) << "\n";
}

// Percobaan Ctrl+Z
static void cobaUndo(Stack& s) {
    int nilai = -999;

    bool berhasil = pop(s, nilai);

    baris("Ctrl+Z");

    if (berhasil)
        cout << "berhasil, yang dibatalkan = " << nilai << "\n";
    else
        cout << "gagal (riwayat kosong), nilai tidak diubah ("
             << nilai << ")\n";
}

static void langkah(const string& teks) {
    cout << "\n" << teks << "\n";
}

int main() {

    cout << "==================================================\n";
    cout << " Study Case — Aplikasi Editor \"Tulis\"\n";
    cout << " Memeragakan satu sesi mengetik\n";
    cout << " (bagian ini tidak ikut dinilai)\n";
    cout << "==================================================\n";

    Stack s;
    inisialisasi(s);

    langkah("[0] Dokumen baru dibuka, riwayat undo masih kosong");
    keadaan(s);

    langkah("[1] SOAL 1 — push: tiga perubahan diketik (10, 20, lalu 30)");

    baris("push 10") << benarSalah(push(s, 10)) << "\n";
    baris("push 20") << benarSalah(push(s, 20)) << "\n";
    baris("push 30") << benarSalah(push(s, 30)) << "\n";

    keadaan(s);

    cout << "\n    Yang benar: display \"30 20 10\", puncak 30, isEmpty false\n";

    langkah("[2] SOAL 1 — tidak ada batas kapasitas: 10 perubahan sekaligus");

    bool semuaMasuk = true;

    for (int i = 1; i <= 10; ++i) {
        if (!push(s, i * 100))
            semuaMasuk = false;
    }

    baris("semua masuk") << benarSalah(semuaMasuk) << "\n";

    keadaan(s);

    cout << "\n    Yang benar: semua masuk true — linked list tidak pernah penuh\n";

    langkah("[3] SOAL 2 — pop: Ctrl+Z, yang dibatalkan harus 1000");

    cobaUndo(s);
    keadaan(s);

    cout << "\n    Yang benar: berhasil dengan nilai 1000\n";

    langkah("[4] SOAL 3 — clear: Ctrl+S, seluruh riwayat undo dibuang");

    clear(s);
    keadaan(s);

    cout << "\n    Yang benar: display \"\", puncak (tidak ada), isEmpty true\n";

    langkah("[5] SOAL 2 — Ctrl+Z pada dokumen yang baru disimpan (underflow)");

    cobaUndo(s);

    cout << "\n    Yang benar: gagal, dan nilainya tetap -999 (tidak disentuh)\n";

    langkah("[6] Tumpukan tetap bisa dipakai lagi sesudah dikosongkan");

    push(s, 7);
    push(s, 8);

    keadaan(s);

    cout << "\n    Yang benar: display \"8 7\"\n";

    langkah("[7] SOAL 4 — kurungSeimbang: pemeriksa kurung pada kode");

    const string contoh[] = {
        "( a + b ) * ( c - d )",
        "{[()]}",
        "",
        "( a + b ) * ( c - d",
        "( a + [ b ) ]",
        ")("
    };

    for (int i = 0; i < 6; ++i) {

        cout << "    \"" << contoh[i] << "\"";

        for (size_t j = contoh[i].size(); j < 24; ++j)
            cout << " ";

        cout << " -> "
             << benarSalah(kurungSeimbang(contoh[i]))
             << "\n";
    }

    cout << "\n    Yang benar: true, true, true, false, false, false\n";

    clear(s);

    cout << "\n==================================================\n";
    cout << " Sesi selesai. Silakan ubah bagian ini untuk\n";
    cout << " mencoba percobaan Anda sendiri.\n";
    cout << "==================================================\n";

    return 0;
}

#endif