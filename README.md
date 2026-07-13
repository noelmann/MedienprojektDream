# What is it?
It is a video player written in C++ based on the QT framework with a RAG LLM-based question answering system(it uses the GROQ API).
It was made as a concept to demonstrate the viability of combining a video player with a LLM to enhance the ability of a user to learn and understand new information received in the form of an educational video in regard to [Project Dream](https://www.th-koeln.de/informatik-und-ingenieurwissenschaften/dream---dezentrale-datenraeume-fuer-mehr-sicherheit-und-souveraenitaet-bei-der-cloud-nutzung_133527.php).


# How to install it.
1. Clone the repository.
2. Build the project with CMake.
3. Unpack the word embedding zip file and move the embedding file into the build folder.
4. Add your GROQ-API Key to an environment variable named "GROQ_API_KEY_DREAM".
5. Run the application. A folder named "videos" will be automatically created.
6. Move the videos to the previously mentioned folder.

# How to use it.
After completing the installation steps you can simply run the application.
On the left you can select the video which you want to view and on the right you can type your questions that will then be answered by an LLM based on the content of the knowledgebase.

# Additional information
The program was created as part of the [Medienprojekt course](https://www.medieninformatik.th-koeln.de/mi-5.0/medieninformatik-bachelor/modulbeschreibungen-bpo5/BA_Medienprojekt/) during the summer semester in 2026(TH Köln).