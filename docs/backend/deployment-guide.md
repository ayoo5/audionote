# Backend Overview

The backend infrastructure is hosted primarily on Google Cloud Run and manages:

* signed upload URL generation,
* cloud storage upload orchestration,
* automatic transcription,
* transcript generation,
* and Google Shared Drive archival.

## Backend Services

### signedurl Service

Responsible for:

* generating signed upload URLs,
* initializing resumable upload sessions,
* organizing upload metadata,
* and validating upload requests.

### autotranscribe Service

Responsible for:

* monitoring uploaded recordings,
* triggering speech-to-text transcription,
* generating transcript files,
* and organizing archival folders.

## Backend Pipeline

<img src="docs/08-assets/diagrams/Backend Pipeline.png" alt="Backend Pipeline"/>

Backend documentation:

* [Backend Overview](docs/04-backend/backend-overview.md)
* [Cloud Run Architecture](docs/04-backend/cloud-run-architecture.md)
* [Signed URL Service](docs/04-backend/signed-url-service.md)
* [Auto-Transcribe Service](docs/04-backend/auto-transcribe-service.md)
* [Speech-to-Text Pipeline](docs/04-backend/speech-to-text-pipeline.md)
* [Deployment Guide](docs/04-backend/deployment-guide.md)

---

# Google Cloud Platform Setup

The backend infrastructure requires:

* Google Cloud Storage
* Google Cloud Run
* Google Speech-to-Text API
* Google Drive API
* Service Accounts and IAM configuration

Detailed setup guides:

* [GCP Project Setup](docs/04-backend/gcp-project-setup.md)
* [Authentication and Security](docs/04-backend/authentication-and-security.md)
* [Deployment Guide](docs/04-backend/deployment-guide.md)
* [Custom Domain Setup](docs/04-backend/custom-domain-setup.md)
* [Shared Drive Integration](docs/04-backend/shared-drive-integration.md)

Useful external references:

* [Google Cloud Run Documentation](https://cloud.google.com/run/docs)
* [Google Speech-to-Text Documentation](https://cloud.google.com/speech-to-text/docs)
* [Google Drive API Documentation](https://developers.google.com/drive)
