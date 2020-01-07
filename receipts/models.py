from django.db import models
from django.contrib.auth.models import User
# Create your models here.
class Receipt(models.Model):
    lot_no = models.CharField(max_length = 3)
    enter_time = models.CharField(max_length = 100)
    exit_time = models.CharField(max_length = 100, blank=True)
    fee = models.FloatField(default=0)
    owner = models.ForeignKey(User, related_name="receipts", on_delete=models.CASCADE, null=True)
    
    