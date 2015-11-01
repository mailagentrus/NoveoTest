[<img src="https://raw.githubusercontent.com/mailagentrus/NoveoTest/master/img/UIExample.jpg" alt="img text"  width="400" height="350"/>]
===============

Тестовое задание для компании Noveo.
Приложение отправляет запрос на поиск ключевых слов на сайт https://itunes.apple.com/search и получет ответ в виде JSON. Результаты поиска отображаются в виде списка в QScrollArea.

Тестовое приложение содержит в себе Core и UI. Оба модуля собираются с испоьзованием CMake :)

Core:
1. ITunesSearchEngine позволяет сформировать запрос с использованием полей: ApiVersion (1 или 2), SearchLimitSize (ограничение выборки результатов), MediaType (книги, видео, музыка и так далее), Keyword (ключевое слово или список ключевых слов для поиска);
2. JSONSearchResultLis, JSONSearchResult обрабатывают ответ с сервера ITunes в формате JSON;
3. ModelInfo, ModelUserView - модели данных;
4. DBTableInfo, DBTableUserView - схемы данных

UI:
1. ResultViewWidget - виджет для отображения одного результата поиска с загрузкой миниатюры с внешнего источника данных;
2. MainWindow - основной виджет, который содержит в себе список результатов поиска.
