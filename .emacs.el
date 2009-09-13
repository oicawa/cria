;;TrueTypeフォントの設定
(cond (window-system
       (set-default-font "Bitstream Vera Sans Mono-10")
       (setq x-select-enable-clipboard t)   ;;;クリップボードと同期する？
       (set-fontset-font (frame-parameter nil 'font)
                         'japanese-jisx0208
                         '("VL ゴシック" . "unicode-bmp")
                         )))

(setq backup-inhibited t)  ;;バックアップファイルを作成しない。
(setq delete-auto-save-files t);;終了時にオートセーブファイルを消す
(tool-bar-mode 0)  ;;ツールバーを消す
(set-scroll-bar-mode 'right)  ;;スクロールバーを右側に表示
(show-paren-mode 1) ;;対応するカッコを光らせる


;;--------------------
;;Anthyの設定
;;--------------------
(load "anthy")
(setq default-input-method "japanese-anthy")
(setq anthy-wide-space " ")
(anthy-load-hiragana-map anthy-alt-char-map)
;;[全角半角]キーで日本語入力を切り替える。
;;ただし、emacsを起動する際にXがこのキーをフックしないように、
;;.bashrc中に、次の一文を記述しておくこと。
;;------------------------------
;;alias emacs='XMODIFIERS=@im=none \emacs'
;;------------------------------
;;デフォルトの設定でも[全角半角]キーで日本語入力モードにすることは可能だが
;;その際
(global-set-key [zenkaku-hankaku] 'toggle-input-method)
;;日本語入力時に”,”キーで"、"が、"."キーで"。"が出てくれなかったので。
(anthy-change-hiragana-map "." "。")
(anthy-change-hiragana-map "," "、")



;;;環境設定
(custom-set-variables
    '(inhibit-startup-message t))


;;;;UI設定
;;(if window-system
;;    (progn
;;        (add-to-list 'default-frame-alist '(foreground-color . "gray"))
;;        (add-to-list 'default-frame-alist '(background-color . "black"))
;;        (add-to-list 'default-frame-alist '(top . 0))
;;        (add-to-list 'default-frame-alist '(left . 400))
;;        (add-to-list 'default-frame-alist '(cursor-color . "gray"))
;;        (set-face-background 'region "LightSteelBlue1")))
;
;
;
;;;;透明度
;;(set-alpha '(80))


;;カーソルを縦棒にする。（効かねー）
(add-to-list 'default-frame-alist '(cursor-type . '(bar . 3)))


;;------------------------------
;;キーバインド設定
;;------------------------------
;;基本のショートカットキー設定
(global-set-key "\C-o" 'find-file)
(global-set-key "\C-w" 'kill-buffer)
(global-set-key "\C-n" 'generate-new-buffer)
(global-set-key "\C-s" 'save-buffer)
(global-set-key "\C-z" 'undo)
;;(global-set-key "\C-F" 're-search-forward)
;(global-set-key "\C-f" 'isearch-forward)
(global-set-key "\C-m" 'goto-line)
;(global-set-key "\C-q" 'save-buffers-kill-emacs)
;
;
;;バッファ操作
(global-set-key [C-tab] 'next-buffer)
(global-set-key [C-S-iso-lefttab] 'previous-buffer)
;(global-set-key "\M-b" 'list-buffers)



;;カーソル移動
(global-set-key "\C-h" 'backward-char)
(global-set-key "\C-j" 'next-line)
(global-set-key "\C-k" 'previous-line)
(global-set-key "\C-l" 'forward-char)
(global-set-key "\C-H" 'beginning-of-line)
;(global-set-key "\C-]" 'end-of-line)


;;貼り付け
(global-set-key "\C-v" 'yank)
;;コピー
(global-set-key "\C-c" 'copy-region-as-kill)
;;カット
(global-set-key "\C-x" 'kill-region)
;
;
;
;
;(local-set-key "\C-p" ctl-x-map)
;
;;ファイル操作
;;(define-prefix-command 'menu-file-map)
;;(global-set-key "\M-f" 'menu-file-map)
;;(define-key menu-file-map "q" 'save-buffers-kill-emacs)
;
;
;;ウィンドウ操作
;;(define-prefix-command 'menu-window-map)
;;(global-set-key "\M-w" 'menu-window-map)
;;(define-key menu-window-map "v" 'split-window-vertically)
;;(define-key menu-window-map "h" 'split-window-horizontally)
;;(define-key menu-window-map "w" 'delete-window)
;;(define-key menu-window-map "o" 'other-window)
;
;
;;;;GDB用の設定
;;(setq gdb-many-windows t)
;;(setq gdb-use-separate-io-buffer t) ; "IO buffer" が必要ない場合は  nil で
;
;
