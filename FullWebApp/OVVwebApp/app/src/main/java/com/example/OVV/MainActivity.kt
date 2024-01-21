package com.example.OVV

import android.os.Bundle
import android.util.Log
import android.webkit.ConsoleMessage
import android.webkit.WebChromeClient
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.activity.ComponentActivity

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val webView = findViewById<WebView>(R.id.webview)

        window.statusBarColor = resources.getColor(R.color.white)

        val sborra = findViewById<WebView>(R.id.webview)
        sborra.settings.javaScriptEnabled = true
        sborra.webViewClient = WebViewClient()
        sborra.webChromeClient = object : WebChromeClient() {
            override fun onConsoleMessage(consoleMessage: ConsoleMessage?): Boolean {
                Log.d("WebView", "JavaScript Console: ${consoleMessage?.message()}")
                return true
            }
        }
        webView.loadUrl("file:///android_asset/index.html")

    }

}