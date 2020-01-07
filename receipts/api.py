from .models import Receipt
from rest_framework import viewsets, permissions
from .serializers import ReceiptSerializer

#Receipt Viewset
class ReceiptViewSet(viewsets.ModelViewSet):
    permission_classes = [
        permissions.IsAuthenticated
        ]
    serializer_class = ReceiptSerializer
    def get_queryset(self):
        return self.request.user.receipts.all()

    def perform_create(self, serializer):
        serializer.save(owner=self.request.user)

    