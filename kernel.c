#include <efi.h>
#include <efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Używamy CHAR16 do komunikatu
    CHAR16 *message = L"Hello, UEFI Kernel!\n";

    // Ustawienie wskaźnika na interfejs wyjścia tekstowego
    EFI_SIMPLE_TEXT_OUT_PROTOCOL *ConOut = SystemTable->ConOut;

    // Sprawdź, czy interfejs wyjścia jest poprawny
    if (ConOut == NULL) {
        return EFI_UNSUPPORTED; // Jeśli nie ma wyjścia, zakończ program
    }

    // Wyświetlenie komunikatu
    ConOut->OutputString(ConOut, message);

    // Zatrzymanie systemu (zatrzymanie CPU)
    while (1) { __asm__("hlt"); }

    return EFI_SUCCESS;
}
