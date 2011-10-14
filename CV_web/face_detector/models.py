from django.db import models

class ModelImage(models.Model):
    posted_on = models.DateTimeField(auto_now_add=True, editable=False)
    img = models.ImageField(upload_to='imgs')