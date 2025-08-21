# VPN Project

This project implements a simple VPN using a TUN interface and UDP sockets.  
It includes both a **server** and a **client** application.

---

## 📦 Requirements
- Linux environment with TUN support
- `gcc` (or compatible C compiler)
- `make`
- Root privileges (required to create/configure TUN device)

---

## 🚀 Installation & Usage

### VPN Server
```bash
cd vpn_server_app
make
chmod +x bash_server.sh
sudo ./bash_server.sh
```

### VPN Client
```bash
cd vpn_client_app
make
chmod +x bash_client.sh
sudo ./bash_client.sh
```

---

## ⚙️ Project Structure
```
.vpn_project/
├── vpn_server_app/
│   ├── vpn_server.c
│   ├── bash_server.sh
│   ├── Makefile
│   └── ...
├── vpn_client_app/
│   ├── vpn_client.c
│   ├── bash_client.sh
│   ├── Makefile
│   └── ...
├── README.md
└── ...
```

---

## 🔑 Notes
- Run both server and client with `sudo` (or as root), since the TUN device requires elevated privileges.
- Make sure server and client are configured with the **same port** and **IP settings**.
- To stop the VPN, terminate the running process (`Ctrl+C`) and clean up with:
  ```bash
  make clean
  ```

---

## 🛠️ Next Steps
- Add support for encryption (e.g., TLS, AES).
- Implement authentication between client and server.
- Extend for multiple clients.
- Add logging and debug levels.

---

## 👨‍💻 Authors
- Moshe Buyum  
- Infinity Labs Training Project