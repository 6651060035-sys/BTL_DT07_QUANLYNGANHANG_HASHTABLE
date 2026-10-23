# BTL_DT07_QUANLYNGANHANG_HASHTABLE
````markdown

\\\\# H? TH?NG QU?N LÝ TÀI KHO?N VÀ GIAO D?CH NGÂN HÀNG

> \\\\\\\*\\\\\\\*?? án H?c ph?n:\\\\\\\*\\\\\\\* C?u trúc D? li?u \\\\\\\& Gi?i thu?t | \\\\\\\*\\\\\\\*Mã ?? Tài:\\\\\\\*\\\\\\\* ?T07  

> \\\\\\\*\\\\\\\*Công ngh? Lõi:\\\\\\\*\\\\\\\* C++11 | B?ng B?m (Hash Table) | Separate Chaining | Polynomial Hash | Auto-Rehash



\\\\---



\\\\## 1. ??T V?N ?? VÀ M?C TIÊU ?? TÀI



Trong các h? th?ng ngân hàng hi?n ??i (Core Banking), t?c ?? ph?n h?i ??i v?i các thao tác truy xu?t d? li?u tài kho?n và th?c hi?n giao d?ch ?óng vai trò vô cùng quan tr?ng. Vi?c s? d?ng m?ng t?nh, danh sách liên k?t ??n thông th??ng v?i ?? ph?c t?p O(n) hay các c?u trúc cây tìm ki?m O(log n) ch?a ??t hi?u n?ng t?i ?u nh?t.



\\\\\\\*\\\\\\\*?? tài ?T07\\\\\\\*\\\\\\\* gi?i quy?t bài toán này b?ng cách xây d?ng \\\\\\\*\\\\\\\*H? th?ng Qu?n lý Tài kho?n \\\\\\\& Giao d?ch Ngân hàng\\\\\\\*\\\\\\\* d?a trên c?u trúc d? li?u \\\\\\\*\\\\\\\*B?ng b?m (Hash Table)\\\\\\\*\\\\\\\*:

\\\\\\\* \\\\\\\*\\\\\\\*T?c ?? tra c?u ??t O(1)\\\\\\\*\\\\\\\* trong tr??ng h?p trung bình nh? gi?i thu?t b?m chu?i t?i ?u.

\\\\\\\* \\\\\\\*\\\\\\\*Gi?i quy?t ??ng ?? tri?t ??\\\\\\\*\\\\\\\* b?ng ph??ng pháp \\\\\\\*\\\\\\\*Separate Chaining\\\\\\\*\\\\\\\* (k?t h?p m?ng con tr? và danh sách liên k?t ??n).

\\\\\\\* \\\\\\\*\\\\\\\*Ki?m soát hi?u n?ng linh ho?t\\\\\\\*\\\\\\\* thông qua thu?t toán \\\\\\\*\\\\\\\*Rehash t? ??ng\\\\\\\*\\\\\\\* khi h? s? t?i (Load Factor) v??t ng??ng alpha >= 0.75.

\\\\\\\* \\\\\\\*\\\\\\\*??m b?o tính toàn v?n d? li?u\\\\\\\*\\\\\\\* trong các nghi?p v? ngân hàng c?t lõi (N?p ti?n, Rút ti?n, Chuy?n kho?n an toàn) và l?u tr? d? li?u b?n v?ng ra file CSV.



\\\\---



\\\\## 2. ??C T? K? THU?T \\\\\\\& GI?I THU?T C?T LÕI



\\\\### 2.1. C?u trúc B?ng B?m \\\\\\\& Separate Chaining

\\\\\\\* \\\\\\\*\\\\\\\*Khóa b?m (Key):\\\\\\\*\\\\\\\* Chu?i ký t? S? tài kho?n ngân hàng (ví d?: "1000000001").

\\\\\\\* \\\\\\\*\\\\\\\*M?ng B?m (Bucket Array):\\\\\\\*\\\\\\\* M?ng ??ng các con tr? `Node\\\\\\\*\\\\\\\* table`, m?i con tr? qu?n lý m?t danh sách liên k?t ??n.

\\\\\\\* \\\\\\\*\\\\\\\*X? lý ??ng ??:\\\\\\\*\\\\\\\* Khi hai s? tài kho?n cho ra cùng m?t ch? s? b?m, nút d? li?u m?i s? ???c n?i vào ??u danh sách liên k?t v?i th?i gian O(1).



\\\\### 2.2. Thu?t toán B?m Chu?i (Polynomial Rolling Hash)

Hàm b?m chuy?n ??i chu?i S? tài kho?n thành ch? s? m?ng nguyên thông qua công th?c tích l?y ?a th?c:



```text

Hash(s) = ( (s\\\\\\\*p^0 + s\\\\\\\*p^1 + ... + s\\\\\\\[n-1]\\\\\\\*p^(n-1)) % M ) % Capacity

```



\\\\\\\* \\\\\\\*\\\\\\\*Trong ?ó:\\\\\\\*\\\\\\\* `p = 31` (s? nguyên t? t?i ?u cho chu?i ch? s?), `M = 10^9 + 9` (mô-?un ch?ng tràn s?) và `Capacity` là kích th??c m?ng b?m hi?n t?i.



\\\\### 2.3. Gi?i thu?t Rehash T? ??ng

\\\\\\\* \\\\\\\*\\\\\\\*H? s? t?i (Load Factor):\\\\\\\*\\\\\\\* `Load Factor = S? l??ng tài kho?n / Kích th??c m?ng b?m`.

\\\\\\\* \\\\\\\*\\\\\\\*?i?u ki?n Tái b?m:\\\\\\\*\\\\\\\* Khi `Load Factor >= 0.75`, h? th?ng t? ??ng kh?i ch?y hàm `rehash()`:

\\\&#x20; 1. M? r?ng kích th??c m?ng b?m: `Capacity\\\\\\\_m?i = Capacity\\\\\\\_c? \\\\\\\* 2 + 1`.

\\\&#x20; 2. C?p phát m?ng con tr? m?i và b?m l?i toàn b? d? li?u t? m?ng c? sang m?ng m?i.

\\\&#x20; 3. Gi?i phóng b? nh? m?ng con tr? c? nh?m t?i ?u tài nguyên.



\\\\---



\\\\## 3. B?NG PHÂN CÔNG NHÂN S? VÀ S? ?? NHÁNH GIT



D? án ???c qu?n lý mã ngu?n t?p trung trên \\\\\\\*\\\\\\\*GitHub\\\\\\\*\\\\\\\* và giám sát ti?n ?? công vi?c minh b?ch qua \\\\\\\*\\\\\\\*Trello\\\\\\\*\\\\\\\*:



| STT | Thành viên | MSSV | Vai trò \\\\\\\& Nhánh Git (Branch) | Nhi?m v? chuyên môn ??m nh?n |

| :-: | :--- | :-: | :--- | :--- |

| \\\\\\\*\\\\\\\*1\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*Phan Nam\\\\\\\*\\\\\\\* \\\\\\\*(Leader)\\\\\\\* | \\\\\\\*\\\\\\\*6651060035\\\\\\\*\\\\\\\* | `main`<br>`TV1-leader-core-hash-table` | • Kh?i t?o Repo Git, qu?n lý ti?n ?? Trello.<br>• Cài ??t Core `HashTable`, Polynomial Hash, Rehash.<br>• Vi?t Ch??ng 2 Báo cáo H?c thu?t. |

| \\\\\\\*\\\\\\\*2\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*Nguy?n Ph??ng Hoàng Tr?ng\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*6651060065\\\\\\\*\\\\\\\* | `TV2-quan-ly-tai-khoan` | • Phát tri?n phân h? M? tài kho?n m?i \\\\\\\& Tra c?u O(1).<br>• B?t l?i ngo?i l? nh?p li?u \\\\\\\& Ki?m tra trùng khóa.<br>• Vi?t Ch??ng 1 Báo cáo H?c thu?t. |

| \\\\\\\*\\\\\\\*3\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*Vòng Th? Quy?n\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*6651060051\\\\\\\*\\\\\\\* | `TV3-nghiep-vu-giao-dich` | • Phát tri?n nghi?p v? N?p, Rút, Chuy?n kho?n an toàn.<br>• Qu?n lý l?ch s? giao d?ch \\\\\\\& xu?t sao kê.<br>• Vi?t Ch??ng 3 Báo cáo (Mã ngu?n C++). |

| \\\\\\\*\\\\\\\*4\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*Hu?nh Anh Tú\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*6651060069\\\\\\\*\\\\\\\* | `TV4-doc-ghi-file-giao-dien` | • ??c/Ghi d? li?u b?n v?ng xu?ng file CSV.<br>• Thi?t k? giao di?n Menu Console t??ng tác.<br>• C?n ch?nh ??nh d?ng Báo cáo 40–50 trang. |

| \\\\\\\*\\\\\\\*5\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*Nguy?n Phú Qu?c\\\\\\\*\\\\\\\* | \\\\\\\*\\\\\\\*6651060049\\\\\\\*\\\\\\\* | `TV5-testing-benchmark-demo` | • Ki?m th? ??n v? (Unit Test) \\\\\\\& Ki?m tra Memory Leak.<br>• Ch?y Benchmark ki?m ch?ng gi?i thu?t Rehash.<br>• So?n Slide thuy?t trình \\\\\\\& Báo cáo Ch??ng 4. |



\\\\---



\\\\## 4. ?ÁNH GIÁ ?? PH?C T?P THU?T TOÁN



| Thao tác nghi?p v? | ?? ph?c t?p Trung bình | ?? ph?c t?p X?u nh?t | Ghi chú k? thu?t |

| :--- | :-: | :-: | :--- |

| \\\\\\\*\\\\\\\*Tra c?u tài kho?n (Search)\\\\\\\*\\\\\\\* | O(1) | O(n) | O(1) nh? Rehash duy trì h? s? t?i < 0.75. |

| \\\\\\\*\\\\\\\*M? tài kho?n m?i (Insert)\\\\\\\*\\\\\\\* | O(1) | O(n) | Chèn tr?c ti?p vào ??u danh sách liên k?t ??n. |

| \\\\\\\*\\\\\\\*Xóa tài kho?n (Delete)\\\\\\\*\\\\\\\* | O(1) | O(n) | Gi?i phóng nút và c?p nh?t liên k?t con tr?. |

| \\\\\\\*\\\\\\\*Tái b?m (Rehash)\\\\\\\*\\\\\\\* | O(n) | O(n) | Kích ho?t t? ??ng khi m?ng b?m ??t ng??ng t?i. |



\\\\---



\\\\## 5. C?U TRÚC MÃ NGU?N D? ÁN



```text

.

??? Account.h               # ??nh ngh?a struct Account \\\\\\\& struct Transaction

??? HashTable.h             # L?p HashTable, Polynomial Hash, Rehash \\\\\\\& Destructor

??? main.cpp                # Ch??ng trình t?ng h?p k?t n?i các module \\\\\\\& Menu Console

??? bank\\\\\\\_data.csv           # File c? s? d? li?u l?u tr? tài kho?n b?n v?ng

??? BaoCao\\\\\\\_BTL\\\\\\\_DT07.docx    # Báo cáo h?c thu?t chi ti?t (40 - 50 trang)

??? README.md               # Tài li?u h??ng d?n và gi?i thi?u t?ng quan d? án

```



\\\\---



\\\\## 6. H??NG D?N BIÊN D?CH VÀ CH?Y CH??NG TRÌNH



\\\\### Yêu c?u môi tr??ng

\\\\\\\* \\\\\\\*\\\\\\\*Trình biên d?ch:\\\\\\\*\\\\\\\* GCC / G++ h? tr? chu?n \\\\\\\*\\\\\\\*C++11\\\\\\\*\\\\\\\* tr? lên.

\\\\\\\* \\\\\\\*\\\\\\\*Môi tr??ng khuy?n ngh?:\\\\\\\*\\\\\\\* Dev-C++, Visual Studio Code, Code::Blocks, ho?c Terminal.



\\\\### 6.1. Biên d?ch b?ng Terminal / Command Prompt

1\\\\. M? Terminal t?i th? m?c ch?a mã ngu?n d? án.

2\\\\. Biên d?ch ch??ng trình b?ng GCC:

\\\&#x20;  ```bash

\\\&#x20;  g++ -std=c++11 main.cpp -o quan\\\\\\\_ly\\\\\\\_ngan\\\\\\\_hang

\\\&#x20;  ```

3\\\\. Kh?i ch?y ?ng d?ng:

\\\&#x20;  \\\\\\\* \\\\\\\*\\\\\\\*Trên Windows:\\\\\\\*\\\\\\\* `quan\\\\\\\_ly\\\\\\\_ngan\\\\\\\_hang.exe`

\\\&#x20;  \\\\\\\* \\\\\\\*\\\\\\\*Trên Linux / macOS:\\\\\\\*\\\\\\\* `./quan\\\\\\\_ly\\\\\\\_ngan\\\\\\\_hang`



\\\\### 6.2. Biên d?ch trên Dev-C++

1\\\\. Kh?i ??ng Dev-C++ và m? file `main.cpp`.

2\\\\. Vào \\\\\\\*\\\\\\\*Tools -> Compiler Options\\\\\\\*\\\\\\\*, tích ch?n \\\\\\\*\\\\\\\*Add the following commands when calling the compiler\\\\\\\*\\\\\\\* và nh?p: `-std=c++11`.

3\\\\. Nh?n phím \\\\\\\*\\\\\\\*F11\\\\\\\*\\\\\\\* (\\\\\\\*\\\\\\\*Compile \\\\\\\& Run\\\\\\\*\\\\\\\*) ?? ch?y ch??ng trình.



\\\\### 6.3. Ch?y tr?c tuy?n trên GDB Online

1\\\\. Truy c?p trang web GDB Online C++ Compiler.

2\\\\. Ch?n ngôn ng? \\\\\\\*\\\\\\\*C++\\\\\\\*\\\\\\\* (C++11).

3\\\\. Dán toàn b? mã ngu?n `main.cpp` vào và ch?n \\\\\\\*\\\\\\\*Run\\\\\\\*\\\\\\\*.



\\\\---



\\\\## 7. QU?N LÝ B? NH? VÀ KI?M TH?



\\\\\\\* \\\\\\\*\\\\\\\*Ki?m soát B? nh? (Memory Leak Free):\\\\\\\*\\\\\\\* Hàm h?y `\\\\\\\~HashTable()` ???c cài ??t ?? gi?i phóng toàn b? các Node trong t?ng danh sách liên k?t tr??c khi thu h?i m?ng con tr? chính `table`.

\\\\\\\* \\\\\\\*\\\\\\\*Ki?m th? Rehash:\\\\\\\*\\\\\\\* Tích h?p tính n?ng Benchmark cho phép chèn hàng lo?t d? li?u m?u ?? ki?m tra kh? n?ng t? ??ng m? r?ng m?ng b?m.



\\\\---



\\\\## 8. THÔNG TIN ?? ÁN

\\\\\\\* \\\\\\\*\\\\\\\*H?c ph?n:\\\\\\\*\\\\\\\* C?u trúc D? li?u \\\\\\\& Gi?i thu?t

\\\\\\\* \\\\\\\*\\\\\\\*D? án:\\\\\\\*\\\\\\\* H? th?ng Qu?n lý Tài kho?n \\\\\\\& Giao d?ch Ngân hàng (?T07)

\\\\\\\* \\\\\\\*\\\\\\\*Ngôn ng?:\\\\\\\*\\\\\\\* C++ (C++11)

\\\\\\\* \\\\\\\*\\\\\\\*N?n t?ng qu?n lý:\\\\\\\*\\\\\\\* GitHub \\\\\\\& Trello

````

---

