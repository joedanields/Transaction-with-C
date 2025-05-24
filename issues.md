
---

````markdown
# 🛠️ Detailed Change Log: Line-by-Line Fixes (Bank Account Management System)

This changelog documents each specific code change made to improve, fix, and enhance the original C program.

---

## ✅ Fixes & Enhancements with Line References

---

### 1. 🔁 Replaced `feof()` Usage in `textFile()`

- **Original (Line ~76):**
  ```c
  while (!feof(readPtr))
````

* **Changed To:**

  ```c
  while ((result = fread(&client, sizeof(struct clientData), 1, readPtr)) == 1)
  ```
* **Why:** Prevents reading garbage data at the end of file.

---

### 2. ❌ Replaced `exit(-1)` With `EXIT_FAILURE`

* **Original (Line \~30):**

  ```c
  exit(-1);
  ```
* **Changed To:**

  ```c
  exit(EXIT_FAILURE);
  ```
* **Why:** `EXIT_FAILURE` is the portable standard defined in `<stdlib.h>`.

---

### 3. 🧾 Added `#include <string.h>`

* **Original:**

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  ```
* **Changed To:**

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  ```
* **Why:** Anticipates future use of string functions like `strcpy`, `strcmp`.

---

### 4. 🗂️ Added File Initialization if `credit.dat` is Missing

* **Inserted after Line \~28:**

  ```c
  if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
      printf("File 'credit.dat' not found. Initializing with blank records...\n");
      initializeFile();
      cfPtr = fopen("credit.dat", "rb+");
      if (cfPtr == NULL) {
          printf("%s: File could not be opened.\n", argv[0]);
          exit(EXIT_FAILURE);
      }
  }
  ```

* **Added Function at Bottom of File (\~Line 245):**

  ```c
  void initializeFile(void) {
      FILE *fPtr = fopen("credit.dat", "wb");
      struct clientData blankClient = {0, "", "", 0.0};
      for (int i = 0; i < 100; ++i) {
          fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
      }
      fclose(fPtr);
  }
  ```

* **Why:** Ensures program runs even if `credit.dat` is missing.

---

### 5. 🧪 Input Validation on `scanf()` for Safety

* **Affected Areas:**

  * `updateRecord()` (\~Line 106)
  * `newRecord()` (\~Line 185)
  * `deleteRecord()` (\~Line 157)

* **Original:**

  ```c
  scanf("%d", &account);
  ```

* **Changed To:**

  ```c
  if (scanf("%u", &account) != 1 || account < 1 || account > 100) {
      printf("Invalid input.\n");
      return;
  }
  ```

* **Why:** Prevents crash or undefined behavior on bad input.

---

### 6. ❗ Fixed fseek() Signed/Unsigned Overflow Warning

* **Original (Line \~112):**

  ```c
  fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
  ```
* **Changed To:**

  ```c
  fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
  ```
* **Why:** Avoids `Woverflow` warning and undefined behavior by casting `size_t` to `long`.

---

## ✅ Resulting Behavior

All changes make the code:

* ⚙️ More robust and production-grade
* 💡 Warning-free when compiled
* 🧠 Safer for learning and real use cases

---

## 📁 Summary Table

| Line No. (Approx) | Change                           | Reason                                 |
| ----------------- | -------------------------------- | -------------------------------------- |
| 3                 | `#include <string.h>`            | Prepares for string operations         |
| 30                | `exit(EXIT_FAILURE);`            | Portable error exit                    |
| 28–38             | Added file initialization logic  | Handles missing `credit.dat`           |
| 76                | Changed `while (!feof(...))`     | Fixed logic bug with file read         |
| 106, 157, 185     | Added input validation           | Prevents invalid input issues          |
| 112               | `fseek(..., -(long)sizeof(...))` | Fixes overflow warning on casting      |
| 245+              | `initializeFile()` function      | Generates empty records if file absent |

---

