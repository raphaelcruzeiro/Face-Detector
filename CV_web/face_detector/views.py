# Create your views here.
from django.http import *
from django.shortcuts import render_to_response, redirect
from django.template.context import RequestContext
from models import ModelImage
from form import *
from django.core.files.base import ContentFile

from face import detect as _detect
import simplejson as json

def index(request):
    if request.method == 'POST':
        image = ModelImage()
        form = ImageForm(request.POST, request.FILES, instance=image)

        print request.FILES

        file_content = ContentFile(request.FILES['img'].read())
        image.img.save(request.FILES['img'].name, file_content)

        image.save()

        print image.img.url

        return render_to_response('face_detector/image.html', RequestContext(request, { 'img' : image.img.url, 'id' : image.id }))

    return render_to_response('face_detector/index.html', RequestContext(request, { 'form' : ImageForm() }))

def detect(request):
    print request.POST

    id = request.POST['id']
    image = ModelImage.objects.get(pk=id)

    return HttpResponse(json.dumps(_detect(image.img.path)))
