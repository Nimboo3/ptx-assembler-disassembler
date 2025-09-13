<div align="center">

# 🛍️ Shopping Website (MERN Monorepo)

Full‑stack food ordering / shopping platform built with the **MERN stack** featuring a customer storefront, admin dashboard, and REST API.

⚡ React (Vite) · 🌐 Express + MongoDB (Mongoose) · 🔐 JWT Auth · 💳 Stripe Payments · 📦 Multer Image Uploads

</div>

---

## 🌟 Overview
A production‑ready foundation for an e‑commerce / food ordering application. Customers can browse items, manage a cart, place & track orders. Admins can manage catalog items and update order statuses. The codebase is organized as a lightweight monorepo with three apps:

| App | Path | Purpose |
|-----|------|---------|
| Frontend | `frontend/` | Customer UI (React) |
| Backend API | `backend/` | REST API, auth, DB, payments |
| Admin Panel | `admin/` | Inventory & order management |

---

## ✅ Feature Summary
### Customer
- Browse categorized products / food items
- Auth: register, login (JWT)
- Persistent cart (server-side)
- Place orders & payment (Stripe)
- View order history & status tracking

### Admin
- Add / list / remove food items (with image upload)
- View all orders & update fulfillment status

### Core / Technical
- RESTful API with Express
- MongoDB Atlas (Mongoose models)
- Image upload via Multer to local `uploads/`
- JWT auth middleware for protected routes
- Stripe integration (payments + verification)
- Modular controllers / routes / models separation

---

## 🏗️ Architecture (High-Level)
```
[React Frontend] --
						  \            (JWT)          +------------------+
							--> [Express API] <------> |  MongoDB (Atlas) |
						  /        |  ^               +------------------+
[React Admin]  -----         |  |  (Multer)
										v  v
									[Uploads/]
										|
									(Static /images)
```

---

## 🧰 Tech Stack
**Frontend / Admin:** React 18, React Router 6, Axios, Vite, Toastify (admin)

**Backend:** Node.js, Express, Mongoose, Multer, JWT, Bcrypt, Stripe, Validator, CORS, Dotenv

**Dev Tooling:** Nodemon, ESLint

---

## 📂 Monorepo Structure
```
root
├── backend/        # API + business logic
├── frontend/       # Customer UI
├── admin/          # Admin dashboard
└── uploads/        # (served via /images) runtime asset store
```

Internal engineering notes excluded for public distribution.

---

## 🚀 Quick Start (Local Development)

### 1. Prerequisites
- Node.js 18+ (LTS recommended)
- MongoDB Atlas account (or local Mongo instance)
- Stripe account (for real payment flow) / test keys

### 2. Clone
```bash
git clone https://github.com/<your-org-or-user>/Shopping-Website.git
cd Shopping-Website
```

### 3. Install Dependencies
```bash
cd backend && npm install
cd ../frontend && npm install
cd ../admin && npm install
```

### 4. Environment Variables (examples)
Backend: `PORT`, `MONGODB_URI`, `JWT_SECRET`, `STRIPE_SECRET_KEY`, optional `STRIPE_WEBHOOK_SECRET`.
Frontend/Admin: `VITE_API_BASE_URL`, `VITE_IMAGE_BASE_URL`, and publishable Stripe key where needed.
All secrets are stored in local `.env` files (never committed).

### 5. Run All Apps (in separate terminals)
```bash
cd backend && npm run server
cd frontend && npm run dev
cd admin && npm run dev
```

### 6. Access
- Customer UI: http://localhost:5173
- Admin Panel: http://localhost:5174 (depending on Vite port)
- API Root: http://localhost:4000/
- Images: http://localhost:4000/images/<filename>

---

## 🔐 Environment & Secrets
Secrets are externalized via `.env` files. Production deployment should use environment config / secret manager. All credentials are excluded from version control.

---

## 🗂️ Data Models (Simplified)
| Model | Fields (key) |
|-------|--------------|
| Food  | name, description, price, image, category |
| User  | name, email (unique), password (hashed), cartData (object) |
| Order | userId, items[array], amount, address, status, date, payment |

---

## 📡 API Overview (Selected Endpoints)
| Area | Method | Endpoint | Description |
|------|--------|----------|-------------|
| Food | POST | `/api/food/add` | Add new food (multipart image) |
| Food | GET  | `/api/food/list` | List all food items |
| Food | POST | `/api/food/remove` | Remove food by id |
| Auth | POST | `/api/user/register` | Register user |
| Auth | POST | `/api/user/login` | Login / get JWT |
| Cart | POST | `/api/cart/add` | Add item (auth) |
| Cart | POST | `/api/cart/remove` | Remove item (auth) |
| Cart | POST | `/api/cart/get` | Get cart (auth) |
| Order| POST | `/api/order/place` | Place new order (auth) |
| Order| POST | `/api/order/userorders` | User's orders (auth) |
| Order| POST | `/api/order/verify` | Verify payment |
| Order| GET  | `/api/order/list` | Admin: list all |
| Order| POST | `/api/order/status` | Admin: update status |

Authentication: Provide `Authorization: Bearer <token>` for protected routes.

---

## 🛡️ Security Considerations
JWT auth, password hashing (bcrypt), input validation, and isolated protected routes are implemented. Additional enterprise enhancements (rate limiting, centralized logging, externalized storage, webhooks) can be layered as needed.

---

## 🧪 Testing
The architecture is test-ready (controllers and routes decoupled). Suggested stack: Jest + Supertest + React Testing Library (future enhancement).

---

## 🤝 Contributing
1. Fork repository
2. Create feature branch: `git checkout -b feat/awesome`
3. Commit changes: `git commit -m "feat: add awesome"`
4. Push branch: `git push origin feat/awesome`
5. Open Pull Request

Follow conventional commits where possible (`feat:`, `fix:`, `docs:` etc.).

---

## 📄 License
Licensed under the ISC License (see individual `package.json` files). You may adapt to MIT if redistributing.

---

## 🙌 Acknowledgements
- Stripe API for payment workflow
- MongoDB Atlas for managed database hosting
- Vite for fast frontend dev experience

---

---

Built as a showcase project highlighting full-stack architecture, clean separation of concerns, and extensibility. 🚀

