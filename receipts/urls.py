from rest_framework import routers
from .api import ReceiptViewSet

router = routers.DefaultRouter()
router.register('api/receipts', ReceiptViewSet, 'receipts')

urlpatterns = router.urls