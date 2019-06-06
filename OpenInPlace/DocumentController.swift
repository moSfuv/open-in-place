//
//  DocumentController.swift
//  OpenInPlace
//
//  Created by Anders Borum on 31/05/2019.
//  Copyright © 2019 Applied Phasor. All rights reserved.
//

import UIKit

class TextDocument : UIDocument {
    var text = ""
    
    override func load(fromContents contents: Any, ofType typeName: String?) throws {
        if let data = contents as? Data,
           let text = String(data: data, encoding: .utf8) {
            
            self.text = text
        }
    }
    
    override func contents(forType typeName: String) throws -> Any {
        return text.data(using: .utf8) ?? Data()
    }
    
    override func handleError(_ error: Error, userInteractionPermitted: Bool) {
        NSLog("TextDocument error: \(error)")
        super.handleError(error, userInteractionPermitted: userInteractionPermitted)
    }
    
    override func save(to url: URL, for saveOperation: UIDocumentSaveOperation,
                       completionHandler: ((Bool) -> Void)? = nil) {
        
        NSLog("TextDocument save: \(url.path)")
        super.save(to: url, for: saveOperation, completionHandler: completionHandler)
    }
}

class DocumentController: UIViewController, UITextViewDelegate {
    
    @IBOutlet var textView: UITextView?
    
    func load() {
        guard isViewLoaded else {
            return
        }
        
        navigationItem.title = document?.localizedName
        textView?.text = document?.text

    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        load()
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        self.url = nil
    }
    
    private var document: TextDocument?
    
    public var url: URL? {
        set {
            if let url = newValue {
                document = TextDocument(fileURL: url)
                document?.open(completionHandler: { ok in
                 
                    let text = self.document?.text
                    DispatchQueue.main.async {
                        self.load()
                    }
                    
                })
            } else {
                document?.close(completionHandler: { _ in })
                document = nil
            }
        }
        get {
            return document?.fileURL
        }
    }
    
    //MARK: - UITextViewDelegate
    
    func textViewDidChange(_ textView: UITextView) {
        document?.text = textView.text
        document?.updateChangeCount(.done)
    }
    
    //MARK: -
}

