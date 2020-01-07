from rest_framework import generics, permissions
from django.contrib.auth.models import User
from rest_framework.response import Response
from knox.models import AuthToken
from .serializers import UserSerializer, RegisterSerializer, LoginSerializer
from django.contrib.auth import authenticate
from rest_framework import serializers
#Register API
class RegisterAPI(generics.GenericAPIView):
    serializer_class = RegisterSerializer

    def post(self, request, *args, **kwargs):
        name = request.data['username']
        em = request.data['email']
        pswd = request.data['password']
        user = User.objects.create_user(name, em, pswd)
        return Response({
            "user": UserSerializer(user, context=self.get_serializer_context()).data,
            "token": AuthToken.objects.create(user)[1]
        })

class LoginAPI(generics.GenericAPIView):

    def post(self, request, *args, **kwargs):
        name = request.data['username']
        print(name)
        pswd = request.data['password']
        print(pswd)
        user = authenticate(username=name, password=pswd)
        if user is not None:
            return Response({
                "user": UserSerializer(user, context=self.get_serializer_context()).data,
                "token": AuthToken.objects.create(user)[1]
            })
        else:
            raise serializers.ValidationError("Incorrect Credentials")

class UserAPI(generics.RetrieveAPIView):
    permission_classes = [
        permissions.IsAuthenticated,    
    ]

    serializer_class = UserSerializer
    
    def get_object(self):
        return self.request.user