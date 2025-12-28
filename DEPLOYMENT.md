# Deployment Guide - Memory Allocator Simulator

This guide will help you deploy the Memory Allocator Simulator to various cloud platforms.

## Prerequisites

- Git installed and configured
- GitHub account with access to the repository
- Appropriate cloud platform account
- Docker (for container-based deployments)

## Option 1: Heroku Deployment (Recommended for Beginners)

### Step 1: Install Heroku CLI
Download from https://devcenter.heroku.com/articles/heroku-cli

### Step 2: Login to Heroku
```bash
heroku login
```

### Step 3: Create Heroku App
```bash
cd your-project-directory
heroku create your-unique-app-name
```

### Step 4: Deploy
```bash
git push heroku main
```

### Step 5: View Your App
```bash
heroku open
```

Your app will be live at: `https://your-unique-app-name.herokuapp.com`

## Option 2: Docker Hub + Cloud Run (Google Cloud)

### Step 1: Build Docker Image Locally
```bash
cd "c:\Users\vvad0\OneDrive\Desktop\Memory Allocator"
docker build -t memory-allocator .
```

### Step 2: Push to Docker Hub
```bash
# Login to Docker Hub
docker login

# Tag image
docker tag memory-allocator YOUR_DOCKER_USERNAME/memory-allocator:latest

# Push
docker push YOUR_DOCKER_USERNAME/memory-allocator:latest
```

### Step 3: Deploy to Google Cloud Run
```bash
gcloud run deploy memory-allocator \
  --image YOUR_DOCKER_USERNAME/memory-allocator:latest \
  --platform managed \
  --region us-central1 \
  --allow-unauthenticated \
  --port 3000
```

## Option 3: AWS Elastic Beanstalk

### Step 1: Install AWS CLI and EB CLI
```bash
pip install awsebcli --upgrade --user
```

### Step 2: Initialize EB App
```bash
cd "c:\Users\vvad0\OneDrive\Desktop\Memory Allocator"
eb init -p docker memory-allocator
```

### Step 3: Create Environment and Deploy
```bash
eb create memory-allocator-env
eb deploy
```

### Step 4: View Application
```bash
eb open
```

## Option 4: Azure Container Instances

### Step 1: Create Resource Group
```bash
az group create --name memory-allocator-rg --location eastus
```

### Step 2: Build and Push Image
```bash
az acr build --registry your-registry-name --image memory-allocator:latest .
```

### Step 3: Deploy Container
```bash
az container create \
  --resource-group memory-allocator-rg \
  --name memory-allocator \
  --image your-registry-name.azurecr.io/memory-allocator:latest \
  --ports 3000 \
  --protocol TCP \
  --cpu 1 \
  --memory 1
```

## Option 5: DigitalOcean App Platform

### Step 1: Connect GitHub Account
Go to https://cloud.digitalocean.com/apps and connect your GitHub account

### Step 2: Create New App
- Click "Create App"
- Select your GitHub repository
- Choose branch (main)
- Select "Autodeploy" for automatic updates

### Step 3: Configure
- Source: GitHub
- Repository: Your fork/clone
- Branch: main
- Dockerfile: Automatically detected

### Step 4: Deploy
Click "Launch App" and DigitalOcean will build and deploy automatically

## Option 6: GitHub Pages (Static Fallback)

If you only want to host the documentation:

1. Go to repository Settings → Pages
2. Select "Deploy from a branch"
3. Choose "main" branch and "/ (root)" folder
4. Your README will be published at: `https://yourusername.github.io/Memory-Allocation-Simulator`

## Monitoring and Maintenance

### View Logs
- **Heroku**: `heroku logs --tail`
- **Docker**: `docker logs container-id`
- **AWS**: CloudWatch Logs
- **Azure**: Container Instances logs
- **GCP**: Cloud Run logs

### Update After Changes
```bash
# Make changes
git add .
git commit -m "Update description"
git push origin main

# Deploy (auto-deploys on most platforms if CI/CD is configured)
git push heroku main  # For Heroku
```

### Monitor Performance
- Check memory usage
- Monitor API response times
- Track allocation operations
- Monitor for errors

## Environment Variables

If using environment variables, set them on your platform:

```bash
# Heroku
heroku config:set NODE_ENV=production

# Docker
docker run -e NODE_ENV=production memory-allocator

# AWS EB
eb setenv NODE_ENV=production
```

## Scaling

### Heroku
```bash
heroku ps:scale web=2
```

### Docker
```bash
docker run -d -p 3001:3000 memory-allocator
docker run -d -p 3002:3000 memory-allocator
```

### Load Balancing
Use your cloud provider's load balancer to distribute traffic.

## Cost Estimation

| Platform | Free Tier | Cost |
|----------|-----------|------|
| Heroku | 550 dyno-hours/month | $7/month after free tier |
| Google Cloud Run | 180,000 GB-seconds/month | Very affordable (~$0.40/million requests) |
| AWS Free Tier | 750 hours EC2 | Varies, roughly $5-20/month |
| Azure | 12 months free | Varies, roughly $5-15/month |
| DigitalOcean | - | $4-24/month |

## Troubleshooting

### Port Already in Use
```bash
# Find process using port 3000
lsof -i :3000
kill -9 PID
```

### Dependencies Not Installing
```bash
# Remove lock file and reinstall
rm package-lock.json
npm install
```

### Docker Build Fails
```bash
# Clean build
docker build --no-cache -t memory-allocator .
```

### Connection Issues
- Check firewall settings
- Verify port 3000 is exposed
- Ensure NODE_ENV is set correctly

## Security Recommendations

1. **Use HTTPS**: All cloud platforms provide SSL/TLS
2. **Environment Variables**: Store sensitive data in env vars, not code
3. **Input Validation**: The server validates allocation sizes
4. **Rate Limiting**: Consider adding rate limiting for production
5. **CORS**: Currently set to allow all origins - restrict in production

## Next Steps

1. Set up continuous deployment with GitHub Actions
2. Add monitoring with services like:
   - NewRelic
   - Datadog
   - Sentry (error tracking)
3. Enable auto-scaling based on traffic
4. Set up custom domain name
5. Add analytics to track learning usage

---

For specific deployment issues, check the cloud provider's documentation or contact their support.
