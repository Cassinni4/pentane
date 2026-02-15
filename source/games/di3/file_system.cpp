#include <cstdint>
#include <string>
#include <vector>
#include <Windows.h>

#include "../../logger.hpp"
#include "../../sunset/sunset.hpp"

namespace di3 {
namespace fs {

using CreateFileA_t = HANDLE (WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
using CreateFileW_t = HANDLE (WINAPI*)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

static CreateFileA_t g_origCreateFileA = nullptr;
static CreateFileW_t g_origCreateFileW = nullptr;


static std::string safe_read_cstr(const char* p) {
    if (!p) return {};
    size_t maxLen = 1024;
    size_t i = 0;
    try {
        for (; i < maxLen; ++i) {
            char c = p[i];
            if (c == '\0') break;
        }
        return std::string(p, i);
    } catch (...) {
        return {};
    }
}

static std::string safe_wcs_to_utf8(const wchar_t* w) {
    if (!w) return {};
    int required = WideCharToMultiByte(CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr);
    if (required <= 0) return {};
    std::string out;
    out.resize(required - 1);
    WideCharToMultiByte(CP_UTF8, 0, w, -1, &out[0], required, nullptr, nullptr);
    return out;
}

static HANDLE WINAPI hooked_CreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    std::string name = safe_read_cstr(lpFileName);
    if (name.empty()) {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "[CreateFileA] Open request: <empty-or-null at %p> flags=0x%08X disp=%u\n",
            (void*)lpFileName, (unsigned)dwFlagsAndAttributes, (unsigned)dwCreationDisposition);
        logger::log(buf);
    } else {
        char buf[1024];
        snprintf(buf, sizeof(buf),
            "[CreateFileA] Open request: %s  flags=0x%08X disp=%u\n",
            name.c_str(), (unsigned)dwFlagsAndAttributes, (unsigned)dwCreationDisposition);
        logger::log(buf);
    }

    HANDLE h = g_origCreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
                                 lpSecurityAttributes, dwCreationDisposition,
                                 dwFlagsAndAttributes, hTemplateFile);

    DWORD err = GetLastError();
    if (h == INVALID_HANDLE_VALUE) {
        char buf[1024];
        snprintf(buf, sizeof(buf),
            "[CreateFileA][MISSING] %s  (GetLastError=%lu) flags=0x%08X disp=%u\n",
            name.empty() ? "<unknown>" : name.c_str(),
            (unsigned long)err,
            (unsigned)dwFlagsAndAttributes,
            (unsigned)dwCreationDisposition);
        logger::log(buf);
    } else {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "[CreateFileA] Open OK: %s\n",
            name.empty() ? "<unknown>" : name.c_str());
        logger::log(buf);
    }

    return h;
}

static HANDLE WINAPI hooked_CreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    std::string name = safe_wcs_to_utf8(lpFileName);
    if (name.empty()) {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "[CreateFileW] Open request: <empty-or-null at %p> flags=0x%08X disp=%u\n",
            (void*)lpFileName, (unsigned)dwFlagsAndAttributes, (unsigned)dwCreationDisposition);
        logger::log(buf);
    } else {
        char buf[1024];
        snprintf(buf, sizeof(buf),
            "[CreateFileW] Open request: %s  flags=0x%08X disp=%u\n",
            name.c_str(), (unsigned)dwFlagsAndAttributes, (unsigned)dwCreationDisposition);
        logger::log(buf);
    }

    HANDLE h = g_origCreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
                                 lpSecurityAttributes, dwCreationDisposition,
                                 dwFlagsAndAttributes, hTemplateFile);

    DWORD err = GetLastError();
    if (h == INVALID_HANDLE_VALUE) {
        char buf[1024];
        snprintf(buf, sizeof(buf),
            "[CreateFileW][MISSING] %s  (GetLastError=%lu) flags=0x%08X disp=%u\n",
            name.empty() ? "<unknown>" : name.c_str(),
            (unsigned long)err,
            (unsigned)dwFlagsAndAttributes,
            (unsigned)dwCreationDisposition);
        logger::log(buf);
    } else {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "[CreateFileW] Open OK: %s\n",
            name.empty() ? "<unknown>" : name.c_str());
        logger::log(buf);
    }

    return h;
}

static bool patch_import_iat(HMODULE hMod, const char* dllName, const char* importName, void* newFunc, void** out_old)
{
    if (!hMod) return false;
    auto base = reinterpret_cast<std::uint8_t*>(hMod);
    auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    auto nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return false;

    auto importDir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir.VirtualAddress == 0) return false;

    auto imports = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base + importDir.VirtualAddress);
    for (; imports->Name != 0; ++imports) {
        const char* curDll = reinterpret_cast<const char*>(base + imports->Name);
        if (_stricmp(curDll, dllName) != 0) continue;

        auto thunkRef = reinterpret_cast<IMAGE_THUNK_DATA*>(base + imports->FirstThunk);
        auto origThunk = reinterpret_cast<IMAGE_THUNK_DATA*>(base + imports->OriginalFirstThunk);

        for (; thunkRef->u1.Function != 0; ++thunkRef, ++origThunk) {
            IMAGE_IMPORT_BY_NAME* ibn = nullptr;
            if (origThunk->u1.AddressOfData) {
                ibn = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base + origThunk->u1.AddressOfData);
            }
            if (!ibn || !ibn->Name) continue;
            if (strcmp(reinterpret_cast<const char*>(ibn->Name), importName) == 0) {
                DWORD oldProtect;
                void** iatEntry = reinterpret_cast<void**>(&thunkRef->u1.Function);
                if (!VirtualProtect(iatEntry, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
                void* old = *iatEntry;
                *out_old = old;
                *iatEntry = newFunc;
                VirtualProtect(iatEntry, sizeof(void*), oldProtect, &oldProtect);
                return true;
            }
        }
    }

    return false;
}

bool init()
{
    HMODULE hExe = GetModuleHandleA(nullptr);
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "EXE base = %p\n", hExe);
        logger::log(buf);
    }

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) {
        logger::log("Could not find kernel32.dll\n");
        return false;
    }

    g_origCreateFileA = reinterpret_cast<CreateFileA_t>(GetProcAddress(hKernel32, "CreateFileA"));
    g_origCreateFileW = reinterpret_cast<CreateFileW_t>(GetProcAddress(hKernel32, "CreateFileW"));

    if (!g_origCreateFileA && !g_origCreateFileW) {
        logger::log("Could not resolve CreateFileA/W\n");
        return false;
    }

    void* oldA = nullptr;
    void* oldW = nullptr;
    bool patchedA = false;
    bool patchedW = false;

    if (g_origCreateFileA) {
        patchedA = patch_import_iat(hExe, "KERNEL32.dll", "CreateFileA", reinterpret_cast<void*>(&hooked_CreateFileA), &oldA);
        if (patchedA) {
            char buf[256];
            snprintf(buf, sizeof(buf), "IAT patched: CreateFileA (old=%p)\n", oldA);
            logger::log(buf);
            if (oldA && oldA != reinterpret_cast<void*>(g_origCreateFileA)) {
                g_origCreateFileA = reinterpret_cast<CreateFileA_t>(oldA);
            }
        } else {
            logger::log("Failed to patch IAT for CreateFileA\n");
        }
    }

    if (g_origCreateFileW) {
        patchedW = patch_import_iat(hExe, "KERNEL32.dll", "CreateFileW", reinterpret_cast<void*>(&hooked_CreateFileW), &oldW);
        if (patchedW) {
            char buf[256];
            snprintf(buf, sizeof(buf), "IAT patched: CreateFileW (old=%p)\n", oldW);
            logger::log(buf);
            if (oldW && oldW != reinterpret_cast<void*>(g_origCreateFileW)) {
                g_origCreateFileW = reinterpret_cast<CreateFileW_t>(oldW);
            }
        } else {
            logger::log("Failed to patch IAT for CreateFileW\n");
        }
    }

    if (!patchedA && !patchedW) {
        logger::log("No IAT hooks installed for CreateFile (none patched)\n");
    }

    logger::log("CreateFile IAT hooking complete.\n");
    return true;
}

} // namespace fs
} // namespace di3
