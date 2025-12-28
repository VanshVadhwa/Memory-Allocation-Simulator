# 🚀 Deploy Online in 5 Minutes - Render.com

**Your Memory Allocator is ready to go live!** Get a shareable link anyone can use instantly.

## Why Render.com?

✅ **Free tier** - No credit card for 6 months  
✅ **Auto-deploy** - Push to GitHub = automatic update  
✅ **Always online** - Reliable hosting  
✅ **Mobile friendly** - Works on phones/tablets  
✅ **Instant sharing** - Just send the link  

---

## 📋 Prerequisites

- GitHub account (free)
- This repository pushed to GitHub
- That's it!

---

## 🎯 Step-by-Step Deployment

### Step 1: Push to GitHub

```bash
git add .
git commit -m "Ready for deployment"
git push origin main
```

### Step 2: Go to Render.com

1. Visit https://render.com
2. Click **"Sign up"** (use GitHub account for easiest setup)
3. Authorize Render to access your GitHub

### Step 3: Create New Service

1. Click **"New+"** → **"Web Service"**
2. Select your **Memory-Allocation-Simulator** repository
3. Click **"Connect"**

### Step 4: Configure Service

Fill in these settings:

| Setting | Value |
|---------|-------|
| **Name** | memory-allocator |
| **Environment** | Node |
| **Build Command** | `npm install` |
| **Start Command** | `npm start` |
| **Plan** | Free |

### Step 5: Deploy

Click **"Create Web Service"** and wait ~2 minutes...

### Step 6: Get Your Link

Once deployed, you'll see a URL like:
```
https://memory-allocator-xxxxx.onrender.com
```

**That's your public link! Share it anywhere!** 🎉

---

## 🔗 Share With Friends

Just send them:
```
https://memory-allocator-xxxxx.onrender.com
```

They can:
✅ Open it in browser  
✅ Use on phone  
✅ Use on laptop  
✅ No installation needed  
✅ No login required  

---

## ⚡ Auto-Updates

Every time you push to GitHub:
```bash
git push origin main
```

Your live website **automatically updates**! ✨

---

## 📊 Monitor Your App

In Render dashboard:
- View live logs
- Check resource usage
- See deployment status
- Monitor uptime

---

## 🆘 Troubleshooting

**App not loading?**
- Check the Logs tab in Render
- Ensure npm start works locally: `npm start`

**Port issues?**
- Render automatically sets PORT env variable
- Our code uses `process.env.PORT || 3000`

**Static files not showing?**
- Ensure public/ folder is in git
- Check it's being served in server.js

---

## 📈 Next Steps

1. **Customize your domain** (Render offers free .onrender.com)
2. **Add custom domain** ($5/month)
3. **Monitor analytics** (if needed)
4. **Share the link widely**

---

## 🎯 You're Done!

Your Memory Allocator is now **live on the internet!**

**Link to share:** Copy from your Render dashboard  
**Updates automatically:** Push to GitHub  
**Always available:** On render.com (free tier)  

---

## Alternative: Railway.app (Also Great)

If you prefer Railway:

1. Go to https://railway.app
2. Click "Start Project"
3. Select "GitHub" and pick your repo
4. Railway auto-detects Node.js
5. Click Deploy

Done! You'll get a URL instantly.

---

## Alternative: Vercel (For Next.js Future)

If you later want to switch to Next.js frontend:

1. Go to https://vercel.com
2. Import your GitHub project
3. Click "Deploy"

Also works great and has generous free tier.

---

**Your app is live!** 🎉 Share that link!
